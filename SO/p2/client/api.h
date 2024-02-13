#ifndef CLIENT_API_H
#define CLIENT_API_H

#include <stddef.h>

typedef struct {
	int return_code; // 0 if the operation was successful, 1 otherwise
	int req_fd; // file descriptor to write requests to
	int resp_fd; // file descriptor to read responses from
	int session_id; // session id from the server
} ems_setup_data;

/// Connects to an EMS server.
/// @param req_pipe_path Path to the name pipe to be created for requests.
/// @param resp_pipe_path Path to the name pipe to be created for responses.
/// @param server_pipe_path Path to the name pipe where the server is listening.
/// @return A ems_setup_data struct with the return code and the file descriptors for requests and responses.
ems_setup_data ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path);

/// Disconnects from an EMS server.
/// @param req_fd File descriptor to write requests to.
/// @param resp_fd File descriptor to read responses from.
/// @param req_pipe_path Path to the name pipe to be created for requests.
/// @param resp_pipe_path Path to the name pipe to be created for responses.
/// @return 0 in case of success, 1 otherwise.
int ems_quit(int req_fd, int resp_fd, char const* req_pipe_path, char const* resp_pipe_path);

/// Creates a new event with the given id and dimensions.
/// @param event_id Id of the event to be created.
/// @param num_rows Number of rows of the event to be created.
/// @param num_cols Number of columns of the event to be created.
/// @param req_fd File descriptor to write requests to.
/// @param resp_fd File descriptor to read responses from.
/// @return 0 if the event was created successfully, 1 otherwise.
int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols, int req_fd, int resp_fd);

/// Creates a new reservation for the given event.
/// @param event_id Id of the event to create a reservation for.
/// @param num_seats Number of seats to reserve.
/// @param xs Array of rows of the seats to reserve.
/// @param ys Array of columns of the seats to reserve.
/// @param req_fd File descriptor to write requests to.
/// @param resp_fd File descriptor to read responses from.
/// @return 0 if the reservation was created successfully, 1 otherwise.
int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys, int req_fd, int resp_fd);

/// Prints the given event to the given file.
/// @param out_fd File descriptor to print the event to.
/// @param event_id Id of the event to print.
/// @param req_fd File descriptor to write requests to.
/// @param resp_fd File descriptor to read responses from.
/// @return 0 if the event was printed successfully, 1 otherwise.
int ems_show(int out_fd, unsigned int event_id, int req_fd, int resp_fd);

/// Prints all the events to the given file.
/// @param out_fd File descriptor to print the events to.
/// @param req_fd File descriptor to write requests to.
/// @param resp_fd File descriptor to read responses from.
/// @return 0 if the events were printed successfully, 1 otherwise.
int ems_list_events(int out_fd, int req_fd, int resp_fd);

#endif  // CLIENT_API_H
