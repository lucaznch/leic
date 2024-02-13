#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "common/io.h"
#include "eventlist.h"
#include "operations.h"

static struct EventList* event_list = NULL;
static unsigned int state_access_delay_us = 0;

/// Gets the event with the given ID from the state.
/// @note Will wait to simulate a real system accessing a costly memory resource.
/// @param event_id The ID of the event to get.
/// @param from First node to be searched.
/// @param to Last node to be searched.
/// @return Pointer to the event if found, NULL otherwise.
static struct Event* get_event_with_delay(unsigned int event_id, struct ListNode* from, struct ListNode* to) {
	struct timespec delay = {0, state_access_delay_us * 1000};
	nanosleep(&delay, NULL);  // Should not be removed

	return get_event(event_list, event_id, from, to);
}

/// Gets the index of a seat.
/// @note This function assumes that the seat exists.
/// @param event Event to get the seat index from.
/// @param row Row of the seat.
/// @param col Column of the seat.
/// @return Index of the seat.
static size_t seat_index(struct Event* event, size_t row, size_t col) { return (row - 1) * event->cols + col - 1; }

int ems_init(unsigned int delay_us) {
	if (event_list != NULL) {
		fprintf(stderr, "EMS state has already been initialized\n");
		return 1;
	}

	event_list = create_list();
	state_access_delay_us = delay_us;

	return event_list == NULL;
}

int ems_terminate() {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_wrlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	free_list(event_list);
	pthread_rwlock_unlock(&event_list->rwl);
	return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_wrlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	if (get_event_with_delay(event_id, event_list->head, event_list->tail) != NULL) {
		fprintf(stderr, "Event already exists\n");
		pthread_rwlock_unlock(&event_list->rwl);
		return 0;
	}

	struct Event* event = malloc(sizeof(struct Event));

	if (event == NULL) {
		fprintf(stderr, "Error allocating memory for event\n");
		pthread_rwlock_unlock(&event_list->rwl);
		return 1;
	}

	event->id = event_id;
	event->rows = num_rows;
	event->cols = num_cols;
	event->reservations = 0;
	
	if (pthread_mutex_init(&event->mutex, NULL) != 0) {
		pthread_rwlock_unlock(&event_list->rwl);
		free(event);
		return 1;
	}
	event->data = calloc(num_rows * num_cols, sizeof(unsigned int));

	if (event->data == NULL) {
		fprintf(stderr, "Error allocating memory for event data\n");
		pthread_rwlock_unlock(&event_list->rwl);
		free(event);
		return 1;
	}

	if (append_to_list(event_list, event) != 0) {
		fprintf(stderr, "Error appending event to list\n");
		pthread_rwlock_unlock(&event_list->rwl);
		free(event->data);
		free(event);
		return 1;
	}

	pthread_rwlock_unlock(&event_list->rwl);
	return 0;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return 1;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return 1;
	}

	struct Event* event = get_event_with_delay(event_id, event_list->head, event_list->tail);

	pthread_rwlock_unlock(&event_list->rwl);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		return 1;
	}

	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr, "Error locking mutex\n");
		return 1;
	}

	for (size_t i = 0; i < num_seats; i++) {
		if (xs[i] <= 0 || xs[i] > event->rows || ys[i] <= 0 || ys[i] > event->cols) {
			fprintf(stderr, "Seat out of bounds\n");
			pthread_mutex_unlock(&event->mutex);
			return 1;
		}
	}

	for (size_t i = 0; i < event->rows * event->cols; i++) {
		for (size_t j = 0; j < num_seats; j++) {
			if (seat_index(event, xs[j], ys[j]) != i) {
				continue;
			}

			if (event->data[i] != 0) {
				fprintf(stderr, "Seat already reserved\n");
				pthread_mutex_unlock(&event->mutex);
				return 0;
			}

			break;
		}
	}

	unsigned int reservation_id = ++event->reservations;

	for (size_t i = 0; i < num_seats; i++) {
		event->data[seat_index(event, xs[i], ys[i])] = reservation_id;
	}

	pthread_mutex_unlock(&event->mutex);
	return 0;
}

show_data ems_show(unsigned int event_id) {
	show_data return_data = {0, 0, 0, NULL};

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return_data.return_code = 1;
		return return_data;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return_data.return_code = 1;
		return return_data;
	}

	struct Event* event = get_event_with_delay(event_id, event_list->head, event_list->tail);

	pthread_rwlock_unlock(&event_list->rwl);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		return_data.return_code = 1;
		return return_data;
	}

	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr, "Error locking mutex\n");
		return_data.return_code = 1;
		return return_data;
	}

	return_data.num_rows = event->rows;
	return_data.num_cols = event->cols;
	return_data.seats = malloc(event->rows * event->cols * sizeof(unsigned int));

	size_t i, j;

	for (i = 0; i < event->rows; i++) {
		for (j = 0; j < event->cols; j++) {
			return_data.seats[i * event->cols + j] = event->data[i * event->cols + j];
		}
	}

	pthread_mutex_unlock(&event->mutex);
	return return_data;
}

list_data ems_list_events() {
	list_data return_data = {0, 0, NULL};

	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return_data.return_code = 1;
		return return_data;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return_data.return_code = 1;
		return return_data;
	}

	struct ListNode* to = event_list->tail;
	struct ListNode* current = event_list->head;

	if (current == NULL) {
		pthread_rwlock_unlock(&event_list->rwl);
		return return_data;
	}

	while (1) { // Count number of events
		return_data.num_events++;

		if (current == to) {
			break;
		}

		current = current->next;
	}

	current = event_list->head; // Reset current

	return_data.events = malloc(return_data.num_events * sizeof(unsigned int)); // Allocate memory for events

	if (return_data.events == NULL) {
		fprintf(stderr, "Error allocating memory for an array of events\n");
		pthread_rwlock_unlock(&event_list->rwl);
		return_data.return_code = 1;
		return return_data;
	}

	memset(return_data.events, 0, return_data.num_events * sizeof(unsigned int)); // Zero out memory
	
	int i = 0;
	while (1) {
		return_data.events[i++] = (current->event)->id;

		if (current == to) {
			break;
		}

		current = current->next;
	}

	pthread_rwlock_unlock(&event_list->rwl);
	return return_data;
}

void ems_aux_show_for_signal(int out_fd, unsigned int event_id) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return;
	}

	struct Event* event = get_event_with_delay(event_id, event_list->head, event_list->tail);

	pthread_rwlock_unlock(&event_list->rwl);

	if (event == NULL) {
		fprintf(stderr, "Event not found\n");
		return;
	}

	if (pthread_mutex_lock(&event->mutex) != 0) {
		fprintf(stderr, "Error locking mutex\n");
		return;
	}

	for (size_t i = 1; i <= event->rows; i++) {
		for (size_t j = 1; j <= event->cols; j++) {
			char buffer[16];
			sprintf(buffer, "%u", event->data[seat_index(event, i, j)]);

			if (print_str(out_fd, buffer)) {
				perror("Error writing to file descriptor");
				pthread_mutex_unlock(&event->mutex);
				return;
			}

			if (j < event->cols) {
				if (print_str(out_fd, " ")) {
					perror("Error writing to file descriptor");
					pthread_mutex_unlock(&event->mutex);
					return;
				}
			}
		}

		if (print_str(out_fd, "\n")) {
			perror("Error writing to file descriptor");
			pthread_mutex_unlock(&event->mutex);
			return;
		}
	}

	pthread_mutex_unlock(&event->mutex);
}

void ems_events_info_for_signal(int out_fd) {
	if (event_list == NULL) {
		fprintf(stderr, "EMS state must be initialized\n");
		return;
	}

	if (pthread_rwlock_rdlock(&event_list->rwl) != 0) {
		fprintf(stderr, "Error locking list rwl\n");
		return;
	}

	struct ListNode* to = event_list->tail;
	struct ListNode* current = event_list->head;

	if (current == NULL) {
		char buff[] = "No events\n";
		if (print_str(out_fd, buff)) {
			perror("Error writing to file descriptor");
			pthread_rwlock_unlock(&event_list->rwl);
			return;
		}

		pthread_rwlock_unlock(&event_list->rwl);
		return;
	}

	while (1) {
		char buff[] = "Event: ";
		if (print_str(out_fd, buff)) {
			perror("Error writing to file descriptor");
			pthread_rwlock_unlock(&event_list->rwl);
			return;
		}

		char id[16];
		sprintf(id, "%u\n", (current->event)->id);
		if (print_str(out_fd, id)) {
			perror("Error writing to file descriptor");
			pthread_rwlock_unlock(&event_list->rwl);
			return;
		}

		ems_aux_show_for_signal(STDOUT_FILENO, (current->event)->id);

		if (current == to) {
			break;
		}

		current = current->next;
	}

	pthread_rwlock_unlock(&event_list->rwl);
}