#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "utils/utils.h"
#include "eventlist.h"

static struct EventList* event_list = NULL;
static unsigned int state_access_delay_ms = 0;

/// Calculates a timespec from a delay in milliseconds.
/// @param delay_ms Delay in milliseconds.
/// @return Timespec with the given delay.
static struct timespec delay_to_timespec(unsigned int delay_ms) {
	return (struct timespec){delay_ms / 1000, (delay_ms % 1000) * 1000000};
}

/// Gets the event with the given ID from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event_id The ID of the event to get.
/// @return Pointer to the event if found, NULL otherwise.
static struct Event* get_event_with_delay(unsigned int event_id) {
	struct timespec delay = delay_to_timespec(state_access_delay_ms);
	nanosleep(&delay, NULL);  // Should not be removed

	return get_event(event_list, event_id);
}

/// Gets the seat with the given index from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event Event to get the seat from.
/// @param index Index of the seat to get.
/// @return Pointer to the seat.
static unsigned int* get_seat_with_delay(struct Event* event, size_t index) {
	struct timespec delay = delay_to_timespec(state_access_delay_ms);
	nanosleep(&delay, NULL);  // Should not be removed

	return &event->data[index];
}

/// Gets the index of a seat.
/// @note This function assumes that the seat exists.
/// @param event Event to get the seat index from.
/// @param row Row of the seat.
/// @param col Column of the seat.
/// @return Index of the seat.
static size_t seat_index(struct Event* event, size_t row, size_t col) { return (row - 1) * event->cols + col - 1; }

int ems_init(unsigned int delay_ms) {
	if (event_list != NULL) {
		fprintf(stderr, "EMS state has already been initialized\n");
		return 1;
	}

	event_list = create_list();
	state_access_delay_ms = delay_ms;

	return event_list == NULL;
}

int ems_terminate() {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}
	
	free_list(event_list);
	return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols, pthread_mutex_t* events_general_mutex) {
	pthread_mutex_lock(events_general_mutex); /// lock the general mutex for events (this mutex is used exclusively for manipulating the events list; in addition, each event has its own read-write lock)

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	if (get_event_with_delay(event_id) != NULL) {
		fprintf(stderr, "Event already exists\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	struct Event* event = malloc(sizeof(struct Event));

	if (event == NULL) {
		fprintf(stderr, "Error: Error allocating memory for event\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}
	
	pthread_rwlock_init(&event->rwlock, NULL); /// it will be used for synchronization between threads accessing the same event
	event->id = event_id;
	event->rows = num_rows;
	event->cols = num_cols;
	event->reservations = 0;
	event->data = malloc(num_rows * num_cols * sizeof(unsigned int));
	

	if (event->data == NULL) {
		fprintf(stderr, "Error: Error allocating memory for event data\n");
		free(event);
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	for (size_t i = 0; i < num_rows * num_cols; i++) {
		event->data[i] = 0;
	}

	if (append_to_list(event_list, event) != 0) {
		fprintf(stderr, "Error: Error appending event to list\n");
		free(event->data);
		free(event);
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
	return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys, pthread_mutex_t* events_general_mutex) {
	pthread_mutex_lock(events_general_mutex); /// lock the general mutex for events (this mutex is used exclusively for manipulating the events list; in addition, each event has its own read-write lock)

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	pthread_rwlock_wrlock(&event->rwlock); /// lock the event-specific rwlock for writing
	unsigned int reservation_id = ++event->reservations;
	pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events cause the individual event rwlock is already locked

	size_t i = 0;
	
	for (; i < num_seats; i++) {
		size_t row = xs[i];
		size_t col = ys[i];

		if (row <= 0 || row > event->rows || col <= 0 || col > event->cols) {
			fprintf(stderr, "Invalid seat\n");
			pthread_rwlock_unlock(&event->rwlock); /// unlock the event-specific rwlock
			break;
		}

		if (*get_seat_with_delay(event, seat_index(event, row, col)) != 0) {
			fprintf(stderr, "Seat already reserved\n");
			pthread_rwlock_unlock(&event->rwlock); /// unlock the event-specific rwlock
			break;
		}

		*get_seat_with_delay(event, seat_index(event, row, col)) = reservation_id;
	}

	// If the reservation was not successful, free the seats that were reserved.
	if (i < num_seats) {
		event->reservations--;
		for (size_t j = 0; j < i; j++) {
			*get_seat_with_delay(event, seat_index(event, xs[j], ys[j])) = 0;
		}
		return 1;
	}

	pthread_rwlock_unlock(&event->rwlock); /// unlock the event-specific rwlock

	return 0;
}

int ems_show(unsigned int event_id, int output_stream, pthread_mutex_t* output_write_mutex, pthread_mutex_t* events_general_mutex) {
	pthread_mutex_lock(events_general_mutex); /// lock the general mutex for events (this mutex is used exclusively for manipulating the events list; in addition, each event has its own read-write lock)

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}
	
	pthread_rwlock_rdlock(&event->rwlock); /// lock the event-specific rwlock for reading
	pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events cause the individual event rwlock is already locked

	pthread_mutex_lock(output_write_mutex); /// lock the output stream for writing

	for (size_t i = 1; i <= event->rows; i++) {
		for (size_t j = 1; j <= event->cols; j++) {
			unsigned int* seat = get_seat_with_delay(event, seat_index(event, i, j));
			char* seat_string = uint_to_string(*seat); /// get the seat id as a string
			
			write(output_stream, seat_string, strlen(seat_string));

			free(seat_string);

			if (j < event->cols) {
				write(output_stream, " ", strlen(" "));
			}
		}

		write(output_stream, "\n", strlen("\n"));
	}

	pthread_mutex_unlock(output_write_mutex); /// unlock the output stream
	pthread_rwlock_unlock(&event->rwlock); /// unlock the event-specific rwlock

	return 0;
}

int ems_list_events(int output_stream, pthread_mutex_t* output_write_mutex, pthread_mutex_t* events_general_mutex) {
	pthread_mutex_lock(events_general_mutex); /// lock the general mutex for events 

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 1;
	}

	pthread_mutex_lock(output_write_mutex); /// lock the output stream for writing

	if (event_list->head == NULL) {
		write(output_stream, "No events\n", strlen("No events\n"));
		pthread_mutex_unlock(output_write_mutex); /// unlock the output stream
		pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events
		return 0;
	}

	struct ListNode* current = event_list->head;
	while (current != NULL) {
		char* event_id_string = uint_to_string((current->event)->id); /// get the event id as a string

		write(output_stream, "Event: ", strlen("Event: "));
		write(output_stream, event_id_string, strlen(event_id_string));
		write(output_stream, "\n", strlen("\n"));
		current = current->next;
		
		free(event_id_string);
	}

	pthread_mutex_unlock(output_write_mutex); /// unlock the output stream
	pthread_mutex_unlock(events_general_mutex); /// unlock the general mutex for events

	return 0;
}

void ems_wait(unsigned int delay_ms) {
	struct timespec delay = delay_to_timespec(delay_ms);
	nanosleep(&delay, NULL);
}
