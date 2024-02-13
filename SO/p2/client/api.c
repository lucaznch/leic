#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "api.h"
#include "common/constants.h"

ems_setup_data ems_setup(char const* req_pipe_path, char const* resp_pipe_path, char const* server_pipe_path) {
	ems_setup_data setup_info = {0, 0, 0, 0}; // setup info to be returned

	int server_fd = open(server_pipe_path, O_WRONLY);
	
	if (server_fd == -1) {
		fprintf(stderr, "Failed to open pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	unlink(resp_pipe_path);
	unlink(req_pipe_path);

	if (mkfifo(req_pipe_path, 0660) != 0) { // creates the request pipe
		fprintf(stderr, "Failed to create request pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	if (mkfifo(resp_pipe_path, 0660) != 0) { // creates the response pipe
		fprintf(stderr, "Failed to create response pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	char op_code = '1'; // the operation code for connect

	ssize_t num_bytes; // used to check if the write/read was successful
	
	num_bytes = write(server_fd, &op_code, sizeof(char)); // writes the operation code

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	char pipe_path[PIPE_PATH_MAX];

	memset(pipe_path, 0, PIPE_PATH_MAX); // clears the pipe path
	strcpy(pipe_path, req_pipe_path); // copies the request pipe path to the pipe path

	num_bytes = write(server_fd, pipe_path, sizeof(char) * PIPE_PATH_MAX); // writes the request pipe path

	if (num_bytes != sizeof(char) * PIPE_PATH_MAX) {
		fprintf(stderr, "Failed to write to pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	memset(pipe_path, 0, PIPE_PATH_MAX); // clears the buffer
	strcpy(pipe_path, resp_pipe_path); // copies the response pipe path to the buffer

	num_bytes = write(server_fd, pipe_path, sizeof(char) * PIPE_PATH_MAX); // writes the response pipe path

	if (num_bytes != sizeof(char) * PIPE_PATH_MAX) {
		fprintf(stderr, "Failed to write to pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	num_bytes = write(server_fd, "\0", 1);

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	int req_fd = open(req_pipe_path, O_WRONLY);

	if (req_fd == -1) {
		fprintf(stderr, "Failed to open pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	int resp_fd = open(resp_pipe_path, O_RDONLY);

	if (resp_fd == -1) {
		fprintf(stderr, "Failed to open pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	int session_id; // session id from the server
	num_bytes = read(resp_fd, &session_id, sizeof(int)); // reads the session id from the server

	if (num_bytes != sizeof(int)) {
		fprintf(stderr, "Failed to read from pipe\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	if (session_id == -1) {
		fprintf(stderr, "Failed to connect to server\n");
		setup_info.return_code = 1;
		return setup_info;
	}

	close(server_fd); // closes the server pipe

	// Fills the setup info:
	setup_info.session_id = session_id;
	setup_info.req_fd = req_fd;
	setup_info.resp_fd = resp_fd;
	setup_info.return_code = 0;

	return setup_info;
}

int ems_quit(int req_fd, int resp_fd, char const* req_pipe_path, char const* resp_pipe_path) { 
	char op_code = '2'; // 2 for quit

	ssize_t num_bytes = write(req_fd, &op_code, sizeof(char)); // writes the operation code

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		return 1;
	}
	
	close(req_fd); // closes the request pipe
	close(resp_fd); // closes the response pipe

	unlink(req_pipe_path); // unlinks the request pipe
	unlink(resp_pipe_path); // unlinks the response pipe

	return 0;
}

int ems_create(unsigned int event_id, size_t num_rows, size_t num_cols, int req_fd, int resp_fd) {
	char op_code = '3'; // 3 for create

	ssize_t num_bytes = write(req_fd, &op_code, sizeof(char)); // writes the operation code

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &event_id, sizeof(unsigned int)); // writes the event id

	if (num_bytes != sizeof(unsigned int)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &num_rows, sizeof(size_t)); // writes the number of rows

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &num_cols, sizeof(size_t)); // writes the number of columns

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	int return_code; // return code from the server

	num_bytes = read(resp_fd, &return_code, sizeof(int)); // reads the return code from the server

	if (num_bytes != sizeof(int)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}
	
	return return_code;
}

int ems_reserve(unsigned int event_id, size_t num_seats, size_t* xs, size_t* ys, int req_fd, int resp_fd) {
	char op_code = '4'; // 4 for reserve

	ssize_t num_bytes = write(req_fd, &op_code, sizeof(char));

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &event_id, sizeof(unsigned int));

	if (num_bytes != sizeof(unsigned int)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &num_seats, sizeof(size_t));

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, xs, sizeof(size_t) * num_seats);

	if (num_bytes != (ssize_t)(sizeof(size_t) * num_seats)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, ys, sizeof(size_t) * num_seats);

	if (num_bytes != (ssize_t)(sizeof(size_t) * num_seats)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	int return_code; // return code from the server
	num_bytes = read(resp_fd, &return_code, sizeof(int)); // reads the return code from the server

	if (num_bytes != sizeof(int)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	return return_code;
}

int ems_show(int out_fd, unsigned int event_id, int req_fd, int resp_fd) {
	char op_code = '5'; // 5 for show

	ssize_t num_bytes = write(req_fd, &op_code, sizeof(char));

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = write(req_fd, &event_id, sizeof(unsigned int));

	if (num_bytes != sizeof(unsigned int)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	int return_code; // return code from the server
	size_t num_rows; // number of rows
	size_t num_cols; // number of columns
	unsigned int *seats; // array of seats

	num_bytes = read(resp_fd, &return_code, sizeof(int)); // reads the return code from the server

	if (num_bytes != sizeof(int)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	if (return_code != 0) {
		return return_code;
	}

	num_bytes = read(resp_fd, &num_rows, sizeof(size_t)); // reads the number of rows from the server

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = read(resp_fd, &num_cols, sizeof(size_t)); // reads the number of columns from the server

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	seats = malloc(sizeof(unsigned int) * num_rows * num_cols); // allocates memory for the seats

	if (seats == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = read(resp_fd, seats, sizeof(unsigned int) * num_rows * num_cols); // reads the seats from the server

	if (num_bytes != (ssize_t)(sizeof(unsigned int) * num_rows * num_cols)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	size_t i, j; // iterators

	for (i = 0; i < num_rows; i++) {
		for (j = 0; j < num_cols; j++) {
			char id[16];
			sprintf(id, "%u", seats[i * num_cols + j]); // converts the seat id to a string
			write(out_fd, id, strlen(id)); // writes the seat id to the output file

			if (j < num_cols - 1) { // writes a space if the seat is not the last seat in the row
				write(out_fd, " ", 1); 
			}
		}

		write(out_fd, "\n", 1);
	}

	free(seats);

	return 0;
}

int ems_list_events(int out_fd, int req_fd, int resp_fd) {
	char op_code = '6'; // 6 for list events
	ssize_t num_bytes = write(req_fd, &op_code, sizeof(char));

	if (num_bytes != sizeof(char)) {
		fprintf(stderr, "Failed to write to pipe\n");
		exit(EXIT_FAILURE);
	}

	int return_code; // return code from the server
	size_t num_events; // number of events
	unsigned int *events; // array of events

	num_bytes = read(resp_fd, &return_code, sizeof(int)); // reads the return code from the server

	if (num_bytes != sizeof(int)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	if (return_code != 0) {
		return return_code;
	}

	num_bytes = read(resp_fd, &num_events, sizeof(size_t)); // reads the number of events from the server

	if (num_bytes != sizeof(size_t)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	events = malloc(sizeof(unsigned int) * num_events); // allocates memory for the events

	if (events == NULL) {
		fprintf(stderr, "Failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}

	num_bytes = read(resp_fd, events, sizeof(unsigned int) * num_events); // reads the events from the server

	if (num_bytes != (ssize_t)(sizeof(unsigned int) * num_events)) {
		fprintf(stderr, "Failed to read from pipe\n");
		exit(EXIT_FAILURE);
	}

	if (num_events == 0) {
		const char* no_events = "No events\n";
		write(out_fd, no_events, strlen(no_events));
	} else {
		size_t i;
		for (i = 0; i < num_events; i++) {
			write(out_fd, "Event: ", 7);
			char id[16];
			sprintf(id, "%u", events[i]);
			write(out_fd, id, strlen(id));
			write(out_fd, "\n", 1);
		}
	}

	free(events);

	return 0;
}
