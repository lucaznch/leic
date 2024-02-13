#ifndef EMS_OPERATIONS_H
#define EMS_OPERATIONS_H

#include <stddef.h>
#include <pthread.h>

/// Initializes the EMS state.
/// @param delay_ms State access delay in milliseconds.
/// @return 0 if the EMS state was initialized successfully, 1 otherwise.
int ems_init(unsigned int delay_ms);

/// Destroys the EMS state.
int ems_terminate();

/// Creates a new event with the given id and dimensions.
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @param events_general_mutex Mutex to safely create events, access the events list, etc.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols, pthread_mutex_t* events_general_mutex);

/// Creates a new reservation for the given event.
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @param events_general_mutex Mutex to safely create events, access the events list, etc.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(unsigned int event_id, size_t num_seats, size_t *xs, size_t *ys, pthread_mutex_t* events_general_mutex);

/// Prints the given event.
/// @param event_id Id of the event to print.
/// @param output_fd file descriptor to write the event.
/// @param output_write_mutex Mutex to safely write to the output file descriptor.
/// @param events_general_mutex Mutex to safely create events, access the events list, etc.
/// @return 0 if the event was printed successfully, 1 otherwise.
int ems_show(unsigned int event_id, int output_fd, pthread_mutex_t* output_write_mutex, pthread_mutex_t* events_general_mutex);

/// Prints all the events.
/// @param output_fd file descriptor to write the events.
/// @param output_write_mutex Mutex to safely write to the output file descriptor.
/// @param events_general_mutex Mutex to safely create events, access the events list, etc.
/// @return 0 if the events were printed successfully, 1 otherwise.
int ems_list_events(int output_fd, pthread_mutex_t* output_write_mutex, pthread_mutex_t* events_general_mutex);

/// Waits for a given amount of time.
/// @param delay_us Delay in milliseconds.
void ems_wait(unsigned int delay_ms);

#endif  // EMS_OPERATIONS_H
