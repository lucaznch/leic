#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include "common/constants.h"
#include "common/io.h"
#include "operations.h"

typedef struct {
	char req_pipe_path[PIPE_PATH_MAX]; // Path to the request pipe
	char resp_pipe_path[PIPE_PATH_MAX]; // Path to the response pipe
} session_info; // Session info struct (will be used to pass info to the threads about the client)

typedef struct {
		session_info** sessions; // Array of session info structs (buffer)
		int* prod_ptr; // Pointer to the next free position in the array
		int* cons_ptr; // Pointer to the next position to be consumed in the array
		int* count; // Number of elements in the buffer
		pthread_mutex_t* mutex; // Mutex to protect the buffer
		pthread_cond_t* canProd; // Condition variable to signal that can produce
		pthread_cond_t* canCons; // Condition variable to signal that can consume
} common_args; // Common arguments for the threads (based on a producer-consumer buffer implementation)

typedef struct {
	int session_id; // Session ID
	common_args *cmmn_args; // Common arguments
} worker_thread_args; // Worker thread arguments

volatile sig_atomic_t sigusr1_received = 0; // global variable to store the number of SIGUSR1 signals received (it will inform the main thread that n SIGUSR1 signals were received)
pthread_mutex_t sigusr1_mutex = PTHREAD_MUTEX_INITIALIZER; // to guarantee that the global variable is accessed atomically

/// @brief The handler routine for the SIGUSR1 signal. It will update the global variable to inform
///        the main thread that a SIGUSR1 signal was received. 
/// @param s the signal
void sigusr1_handler(int s) { // handler routine for the SIGUSR1 signal
	(void)s; // unused parameter
	sigusr1_received = sigusr1_received + 1; // change the status to confirm there was a signal, and let the main thread deal with it outside this routine
	signal(SIGUSR1, sigusr1_handler); // re-associate the SIGUSR1 signal to this routine
}

/// @brief The handler routine for the SIGPIPE signal. It will print a message and let the code continue
///        to run normally. To prevent the server threads from closing when the client closes the pipe.
/// @param s the signal
void sigpipe_handler(int s) { // handler routine for the SIGPIPE signal (to prevent the server from closing when the client closes the pipe)
	(void)s; // unused parameter
	const char* msg = "SIGPIPE signal received. Maybe the client closed the pipe?\n";
	write(STDERR_FILENO, msg, strlen(msg)); // writes the message to stderr
	signal(SIGPIPE, sigpipe_handler); // re-associate the SIGPIPE signal to this routine
	// The code will continue to run normally because the code is well prepared to deal with errors on write/read operations
}

/// @brief The routine of the worker thread. It will wait for a session to be available in the buffer,
///        and then it will process the requests of the client. When a client quits, the thread will
///        wait for another session to be available in the buffer.
/// @param worker_thread_args args The arguments of the thread.
/// @return void* ALthough it returns void*, it will never return anything
void* worker_thread(void* args) {
	int session_id = ((worker_thread_args*)args)->session_id; // gets the session id of the thread (it does not change)
	common_args* cmmn_args = ((worker_thread_args*)args)->cmmn_args; // gets the common arguments

	ssize_t num_bytes; // used in some read/write operations

	// For handling the SIGPIPE signal:
	signal(SIGPIPE, sigpipe_handler);

	// For blocking the SIGUSR1 signal in this thread:
	sigset_t mask; // represents a set of signals to block in this thread
	sigemptyset(&mask); // initialize the set to an empty set
	sigaddset(&mask, SIGUSR1); // add the SIGUSR1 signal to the set
	pthread_sigmask(SIG_BLOCK, &mask, NULL); // block the SIGUSR1 signal in this thread

	session_info* current_session_info = NULL; // to store the current session info

	while (1) {
		// A producer-consumer slot distribution implementation:
		pthread_mutex_lock(cmmn_args->mutex); // locks the mutex
		while (*(cmmn_args->count) == 0) { // if the buffer is empty
			pthread_cond_wait(cmmn_args->canCons, cmmn_args->mutex); // waits until a slot is available for consumption
		}
		current_session_info = cmmn_args->sessions[*(cmmn_args->cons_ptr)]; // gets the session from the buffer
		*(cmmn_args->cons_ptr) = *(cmmn_args->cons_ptr) + 1; // updates the index of the next slot to be consumed
		if (*(cmmn_args->cons_ptr) == MAX_SESSION_COUNT) {
			*(cmmn_args->cons_ptr) = 0; // resets the index of the next slot to be consumed
		}
		(*(cmmn_args->count)) = (*(cmmn_args->count)) - 1;
		pthread_cond_signal(cmmn_args->canProd); // signals that a slot is available for production
		pthread_mutex_unlock(cmmn_args->mutex); // unlocks the mutex

		if (current_session_info == NULL) { // if the session is NULL, then the buffer is empty
			continue;
		}

		int req_fd = open(current_session_info->req_pipe_path, O_RDONLY); // opens the request pipe

		if (req_fd == -1) { // if the request pipe could not be opened, then it does not exist
			fprintf(stderr, "Failed to open pipe on session %d\n", session_id);
			continue;
		}

		int resp_fd = open(current_session_info->resp_pipe_path, O_WRONLY); // opens the response pipe

		if (resp_fd == -1) { // if the response pipe could not be opened, then it does not exist
			fprintf(stderr, "Failed to open pipe on session %d\n", session_id);
			continue;
		}

		num_bytes = write(resp_fd, &session_id, sizeof(int)); // sends the session id to the client

		if (num_bytes != sizeof(int)) { // if the number of bytes written is not sizeof(int), then the pipe is full
			fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
			continue;
		}

		int to_continue = 1; // to store if the session should continue or not
		while (to_continue) {
			char op_code; // to store the type of operation
			
			num_bytes = read(req_fd, &op_code, sizeof(op_code)); // reads the type of operation

			if (num_bytes != 1) { // if the number of bytes read is not 1, then the pipe is empty
				fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
				int return_code = 1; // 1 means that the operation failed
				write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
				to_continue = 0;
				break;
			}

			switch (op_code) {
				case '2': { // the client wants to quit
					printf("Session %d quitting\n", session_id);
					to_continue = 0;
					break;
				}

				case '3': { // the client wants to create an event
					printf("Session %d creating event\n", session_id);
					unsigned int event_id;
					size_t num_rows;
					size_t num_cols;
					
					num_bytes = read(req_fd, &event_id, sizeof(event_id)); // reads the event id

					if (num_bytes != sizeof(event_id)) { // if the number of bytes read is not sizeof(event_id), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						to_continue = 0;
						break;
					}

					num_bytes = read(req_fd, &num_rows, sizeof(num_rows)); // reads the number of rows

					if (num_bytes != sizeof(num_rows)) { // if the number of bytes read is not sizeof(num_rows), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						to_continue = 0;
						break;
					}

					num_bytes = read(req_fd, &num_cols, sizeof(num_cols)); // reads the number of columns

					if (num_bytes != sizeof(num_cols)) { // if the number of bytes read is not sizeof(num_cols), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						to_continue = 0;
						break;
					}

					int return_code = ems_create(event_id, num_rows, num_cols); // creates the event

					if (return_code  != 0) { // creates the event
						fprintf(stderr, "Failed to create event on session %d\n", session_id);
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						break; // it will continue the session although the event could not be created
					}

					num_bytes = write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe

					if (num_bytes != sizeof(int)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					break;
				}

				case '4': { // the client wants to reserve seats
					printf("Session %d reserving seats\n", session_id);
					unsigned int event_id;
					size_t num_seats;
					size_t* xs;
					size_t* ys;

					num_bytes = read(req_fd, &event_id, sizeof(event_id)); // reads the event id

					if (num_bytes != sizeof(event_id)) { // if the number of bytes read is not sizeof(event_id), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
						to_continue = 0;
						break;
					}

					num_bytes = read(req_fd, &num_seats, sizeof(num_seats)); // reads the number of seats

					if (num_bytes != sizeof(num_seats)) { // if the number of bytes read is not sizeof(num_seats), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
						to_continue = 0;
						break;
					}

					xs = malloc(sizeof(size_t) * num_seats); // allocates memory for the x coordinates

					if (xs == NULL) { // if the memory could not be allocated
						fprintf(stderr, "Failed to allocate memory\n");
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						exit(EXIT_FAILURE);
					}

					ys = malloc(sizeof(size_t) * num_seats); // allocates memory for the y coordinates

					if (ys == NULL) { // if the memory could not be allocated
						fprintf(stderr, "Failed to allocate memory\n");
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						exit(EXIT_FAILURE);
					}

					num_bytes = read(req_fd, xs, sizeof(size_t) * num_seats); // reads the x coordinates

					if (num_bytes != (ssize_t)(sizeof(size_t) * num_seats)) { // if the number of bytes read is not sizeof(size_t) * num_seats, then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
						to_continue = 0;
						break;
					}

					num_bytes = read(req_fd, ys, sizeof(size_t) * num_seats); // reads the y coordinates

					if (num_bytes != (ssize_t)(sizeof(size_t) * num_seats)) { // if the number of bytes read is not sizeof(size_t) * num_seats, then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
						to_continue = 0;
						break;
					}

					int return_code = ems_reserve(event_id, num_seats, xs, ys); // reserves the seats

					free(xs); // frees the memory allocated for the x coordinates
					free(ys); // frees the memory allocated for the y coordinates

					if (return_code != 0) {
						fprintf(stderr, "Failed to reserve seats on session %d\n", session_id);
						write(resp_fd, &return_code, sizeof(int)); // writes the response to the response pipe
						break; // it will continue the session although the seats could not be reserved
					}
					
					num_bytes = write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe

					if (num_bytes != sizeof(int)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					break;
				}

				case '5': { // the client wants to show an event
					printf("Session %d showing event\n", session_id);
					unsigned int event_id;

					num_bytes = read(req_fd, &event_id, sizeof(event_id)); // reads the event id

					if (num_bytes != sizeof(event_id)) { // if the number of bytes read is not sizeof(event_id), then the pipe is empty
						fprintf(stderr, "Failed to read from pipe on session %d\n", session_id);
						int return_code = 1;
						write(resp_fd, &return_code, sizeof(int)); // writes the return code to the response pipe
						to_continue = 0;
						break;
					}

					show_data data = ems_show(event_id); // gets the data needed to show the event

					if (data.return_code != 0) { // if the event could not be shown
						fprintf(stderr, "Failed to show event on session %d\n", session_id);
						write(resp_fd, &data.return_code, sizeof(int)); // writes the return code to the response pipe
						break; // it will continue the session although the event could not be shown
					}

					num_bytes = write(resp_fd, &data.return_code, sizeof(int)); // writes the response to the response pipe

					if (num_bytes != sizeof(int)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					num_bytes = write(resp_fd, &data.num_rows, sizeof(data.num_rows)); // writes the number of rows to the response pipe

					if (num_bytes != sizeof(data.num_rows)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					num_bytes = write(resp_fd, &data.num_cols, sizeof(data.num_cols)); // writes the number of columns to the response pipe

					if (num_bytes != sizeof(data.num_cols)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					num_bytes = write(resp_fd, data.seats, sizeof(unsigned int) * data.num_rows * data.num_cols); // writes a vector with the seats to the response pipe
					
					if (num_bytes != (ssize_t)(sizeof(unsigned int) * data.num_rows * data.num_cols)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
					}

					break;
				}

				case '6': { // the client wants to list the events
					printf("Session %d listing events\n", session_id);

					list_data data = ems_list_events(); // gets the data needed to list the events

					if (data.return_code != 0) { // if the events could not be listed
						fprintf(stderr, "Failed to list events on session %d\n", session_id);
						write(resp_fd, &data.return_code, sizeof(int)); // writes the return code to the response pipe
						break; // it will continue the session although the events could not be listed
					}

					num_bytes = write(resp_fd, &data.return_code, sizeof(int)); // writes the return code to the response pipe

					if (num_bytes != sizeof(int)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					num_bytes = write(resp_fd, &data.num_events, sizeof(data.num_events)); // writes the number of events to the response pipe

					if (num_bytes != sizeof(data.num_events)) {
						fprintf(stderr, "Failed to write to pipe on session %d\n", session_id);
						to_continue = 0;
						break;
					}

					if (data.num_events == 0) { // if the number of events is 0, then there is nothing else to write
						break;
					}

					num_bytes = write(resp_fd, data.events, sizeof(unsigned int) * data.num_events); // writes a vector with the events to the response pipe

					if (num_bytes != (ssize_t)(sizeof(unsigned int) * data.num_events)) {
						fprintf(stderr, "Failed to write to pipe\n");
						to_continue = 0;
					}

					break;
				}

				default: { // the client sent an invalid operation
					fprintf(stderr, "Invalid operation on session %d\n", session_id);
					to_continue = 0;
					break;
				}
			}
		}

		close(req_fd); // closes the request pipe
		close(resp_fd); // closes the response pipe
		unlink(current_session_info->req_pipe_path); // unlinks the request pipe
		unlink(current_session_info->resp_pipe_path); // unlinks the response pipe
	}

	// The code below is never reached (but it is here)
	free(args);
	return NULL;
}

int main(int argc, char* argv[]) {
	if (argc < 2 || argc > 3) { // if the number of arguments is not 2 or 3
		fprintf(stderr, "Usage: %s\n <pipe_path> [delay]\n", argv[0]);
		return 1;
	}

	char* endptr;
	unsigned int state_access_delay_us = STATE_ACCESS_DELAY_US; // default value
	if (argc == 3) {
		unsigned long int delay = strtoul(argv[2], &endptr, 10);

		if (*endptr != '\0' || delay > UINT_MAX) {
			fprintf(stderr, "Invalid delay value or value too large\n");
			return 1;
		}

		state_access_delay_us = (unsigned int)delay;
	}

	if(ems_init(state_access_delay_us) != 0) { // initializes the EMS
		fprintf(stderr, "Failed to initialize EMS\n");
		return 1;
	}

	unlink(argv[1]);

	if (mkfifo(argv[1], 0666) == -1) { // creates the FIFO
		fprintf(stderr, "Failed to create FIFO\n");
		return 1;
	}

	if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) { // associates the SIGUSR1 signal to the handler routine
		fprintf(stderr, "Failed to associate SIGUSR1 signal to handler routine\n");
		return 1;
	}

	int server_fd; // to store the file descriptor of the FIFO
	while ((server_fd = open(argv[1], O_RDONLY)) == -1) { // tries to open the FIFO until it succeeds (to prevent the server from starting before the client)
		if (errno != EINTR) { // if the FIFO could not be opened because of another reason
			fprintf(stderr, "Failed to open FIFO\n");
			return 1;
		}
		
		// If the FIFO could not be opened because of a signal, then we list the events (that will be none)
		pthread_mutex_lock(&sigusr1_mutex); // locks the mutex of the SIGUSR1 signal flag
		while (sigusr1_received != 0) { // in the case that we received more than one SIGUSR1 at the same time
			ems_events_info_for_signal(STDOUT_FILENO); // if we received SIGUSR1 while waiting for the client, then we list the events (that will be none)
			sigusr1_received = sigusr1_received - 1;
		}
		pthread_mutex_unlock(&sigusr1_mutex); 
	}
	
	pthread_t threads[MAX_SESSION_COUNT]; // to store the threads for each session
	session_info* buffer[MAX_SESSION_COUNT]; // to store the information of each session
	memset(buffer, 0, sizeof(buffer)); // initializes the buffer to 0

	int prod_ptr = 0; // to store the index of the next slot to be produced
	int cons_ptr = 0; // to store the index of the next slot to be consumed
	int count = 0; // to store the number of slots occupied
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // to control access to the buffer
	pthread_cond_t canProd = PTHREAD_COND_INITIALIZER; // to signal that a slot is available for production
	pthread_cond_t canCons = PTHREAD_COND_INITIALIZER; // to signal that a slot is available for consumption

	common_args* cmmn_args = malloc(sizeof(common_args)); // allocates memory for the arguments of the thread
	cmmn_args->sessions = buffer;
	cmmn_args->prod_ptr = &prod_ptr;
	cmmn_args->cons_ptr = &cons_ptr;
	cmmn_args->count = &count;
	cmmn_args->mutex = &mutex;
	cmmn_args->canProd = &canProd;
	cmmn_args->canCons = &canCons;

	for (int i = 1; i <= MAX_SESSION_COUNT; i++) { // initializes the sessions
		worker_thread_args* args = malloc(sizeof(worker_thread_args)); // allocates memory for the arguments of the thread

		if (args == NULL) { // if the memory could not be allocated
			fprintf(stderr, "Failed to allocate memory\n");
			return 1;
		}

		args->session_id = i;
		args->cmmn_args = cmmn_args;
		pthread_create(&threads[i - 1], NULL, worker_thread, args); // creates the thread
	}


	char op_code; // to store the type of operation
	ssize_t bytes_read; // to store the number of bytes read
	while (1) {
		pthread_mutex_lock(&sigusr1_mutex);
		while (sigusr1_received != 0) { // if there is one or more SIGUSR1 signals received
			ems_events_info_for_signal(STDOUT_FILENO); // print in stdout a list of events IDs along with its seat status
			sigusr1_received = sigusr1_received - 1; // update SIGUSR1 flag
		}
		pthread_mutex_unlock(&sigusr1_mutex);   

		bytes_read = read(server_fd, &op_code, sizeof(op_code)); // reads the type of operation

		if (bytes_read != 1 || op_code == 0) { // the pipe is empty
			continue;
		}

		session_info* session = malloc(sizeof(session_info)); // allocates memory for the session

		bytes_read = read(server_fd, session->req_pipe_path, sizeof(char) * PIPE_PATH_MAX); // reads the request pipe path

		if (bytes_read != PIPE_PATH_MAX) { // if the number of bytes read is not PIPE_PATH_MAX, then the pipe is empty
			fprintf(stderr, "Failed to read from pipe\n");
			return 1;
		}

		bytes_read = read(server_fd, session->resp_pipe_path, sizeof(char) * PIPE_PATH_MAX); // reads the response pipe path

		if (bytes_read != PIPE_PATH_MAX) { // if the number of bytes read is not PIPE_PATH_MAX, then the pipe is empty
			fprintf(stderr, "Failed to read from pipe\n");
			return 1;
		}

		pthread_mutex_lock(&mutex); // locks the mutex
		while (count == MAX_SESSION_COUNT) { // if the buffer is full
			pthread_cond_wait(&canProd, &mutex); // waits until a slot is available for production
		}
		buffer[prod_ptr] = session; // stores the session in the buffer
		prod_ptr++;
		if (prod_ptr == MAX_SESSION_COUNT) { // if the index of the next slot to be produced is MAX_SESSION_COUNT, then it wraps around
			prod_ptr = 0;
		}
		count++;
		pthread_cond_signal(&canCons); // signals that a slot is available for consumption
		pthread_mutex_unlock(&mutex); // unlocks the mutex
	}
	
	close(server_fd); // closes the FIFO
	unlink(argv[1]); // removes the FIFO
	ems_terminate(); // terminates the EMS
	return 0;
}