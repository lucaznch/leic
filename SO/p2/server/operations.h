#ifndef SERVER_OPERATIONS_H
#define SERVER_OPERATIONS_H

#include <stddef.h>

typedef struct {
	int return_code; // 0 if the operation was successful, 1 otherwise
	size_t num_rows; // number of rows of the event
	size_t num_cols; // number of columns of the event
	unsigned int *seats; // array of seats
} show_data; // struct to return the data of the show operation

typedef struct {
	int return_code; // 0 if the operation was successful, 1 otherwise
	size_t num_events; // number of events
	unsigned int* events; // array of events
} list_data; // struct to return the data of the list operation

/// Initializes the EMS state.
/// @param delay_us Delay in microseconds.
/// @return 0 if the EMS state was initialized successfully, 1 otherwise.
int ems_init(unsigned int delay_us);

/// Destroys the EMS state.
int ems_terminate();

/// Creates a new event with the given id and dimensions.
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols);

/// Creates a new reservation for the given event.
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys);

/// Prints the given event.
/// @param event_id Id of the event to print.
/// @return a show_data struct with the return code, the number of rows and columns and the seats.
show_data ems_show(unsigned int event_id);

/// Prints all the events.
/// @return a list_data struct with the return code, the number of events and the events
list_data ems_list_events();

/// Prints all the events.
/// @param out_fd File descriptor to print the events to.
void ems_events_info_for_signal(int out_fd);

/// Prints the given event.
/// @param out_fd File descriptor to print the event to.
/// @param event_id Id of the event to print.
void ems_aux_show_for_signal(int out_fd, unsigned int event_id);

#endif  // SERVER_OPERATIONS_H
