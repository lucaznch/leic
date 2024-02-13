#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <dirent.h> /// for directory related functions
#include <fcntl.h> /// flags for open, etc.

#include <sys/stat.h> /// permission-related constants
#include <sys/types.h>
#include <sys/wait.h>

#include <pthread.h>
#include <semaphore.h> 

#include "../operations.h"
#include "../utils/utils.h"
#include "../parser.h"
#include "../constants.h"
#include "processing.h"
#include "parallel_processing_utils.h"

#define EXTENSION_TO_PROCESS ".jobs"
#define OUTPUT_EXTENSION ".out"

void* process_file(void* args) {
	thread_args* args_data = (thread_args*) args;

	unsigned int event_id, delay;
	size_t num_rows, num_columns, num_coords;
	size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];
	
	int line_num = 1; /// the line number which is currently being read
	int command; /// the command read from the input fd
	int to_continue = 1; /// indicates if the while loop has finished or not
	
	while (to_continue) {
		command = get_next(args_data->input_fd);
		
		/// check if the current line should be processed by this thread or not
		int should_process = (line_num % args_data->number_of_threads == args_data->thread_id) || (line_num % args_data->number_of_threads == 0 && args_data->thread_id == args_data->number_of_threads);

		/// All the synchronization is done inside the functions called below (ems_create, ems_reserve, etc.) except for the barrier command
		switch (command) {
			case CMD_CREATE:
				if(should_process) {
					if (parse_create(args_data->input_fd, &event_id, &num_rows, &num_columns) != 0) {
						fprintf(stderr, "Invalid command. See HELP for usage\n");
						break;
					}
					if (ems_create(event_id, num_rows, num_columns, &args_data->shared_data->events_general_mutex)) {
						fprintf(stderr, "Failed to create event\n");
					}
				} else {
					cleanup(args_data->input_fd); /// pass to the next line
				}
			break;

			case CMD_RESERVE:
				if(should_process) {
					num_coords = parse_reserve(args_data->input_fd, MAX_RESERVATION_SIZE, &event_id, xs, ys);

					if (num_coords == 0) {
						fprintf(stderr, "Invalid command. See HELP for usage\n");
						break;
					}
					if (ems_reserve(event_id, num_coords, xs, ys, &args_data->shared_data->events_general_mutex)) {
						fprintf(stderr, "Failed to reserve seats\n");
					}
				} else {
					cleanup(args_data->input_fd); /// pass to the next line
				}
			break;

			case CMD_SHOW:
				if(should_process) {
					if (parse_show(args_data->input_fd, &event_id) != 0) {
						fprintf(stderr, "Invalid command. See HELP for usage\n");
						break;
					}
					if (ems_show(event_id, args_data->output_fd, &args_data->shared_data->output_write_mutex, &args_data->shared_data->events_general_mutex)) {
						fprintf(stderr, "Failed to show event\n");
					}
				} else {
					cleanup(args_data->input_fd); /// pass to the next line
				}
			break;

			case CMD_LIST_EVENTS:
				if (should_process && ems_list_events(args_data->output_fd, &args_data->shared_data->output_write_mutex, &args_data->shared_data->events_general_mutex)) {
					fprintf(stderr, "Failed to list events\n");
				}
			break;
			
			case CMD_WAIT: {
				unsigned int parsed_thread_id; /// the thread id parsed from the input file
				int have_thread_id = parse_wait(args_data->input_fd, &delay, &parsed_thread_id);
				
				if (have_thread_id == -1) { /// if the command is invalid
					fprintf(stderr, "Invalid command. See HELP for usage\n");
				} else if (delay > 0) {
					if (!have_thread_id) { /// if the command doesn't have a specified thread id all threads will execute it
						fprintf(stdout, "Waiting...\n");
						ems_wait(delay);
					} else if ((int) parsed_thread_id == args_data->thread_id) { /// if the command has a thread id only the specified thread will execute it
						fprintf(stdout, "Waiting...\n");
						ems_wait(delay);
					}
				}
			}
			break;

			case CMD_INVALID:
				if (should_process) { /// just to avoid printing the error message multiple times
					fprintf(stderr, "Invalid command. See HELP for usage\n");
				}
			break;

			case CMD_HELP:
				if (should_process) {
					const char* help_message = "Available commands:\n"
												"  CREATE <event_id> <num_rows> <num_columns>\n"
												"  RESERVE <event_id> [(<x1>,<y1>) (<x2>,<y2>) ...]\n"
												"  SHOW <event_id>\n"
												"  LIST\n"
												"  WAIT <delay_ms> [thread_id]\n"
												"  BARRIER\n"
												"  HELP\n";
			
					write(args_data->input_fd, help_message, strlen(help_message));
				}
			break;

			case CMD_BARRIER:
				/// For this to work we need to use 2 semaphores (one initialized with 0 and one with 1) and a mutex...
				pthread_mutex_lock(&args_data->shared_data->barrier_mod_mutex); /// get exclusive access to the shared variables
				args_data->shared_data->blocked_threads_counter++;
				if (args_data->shared_data->blocked_threads_counter == args_data->number_of_threads) { /// if all threads have reached the barrier
					sem_wait(&args_data->shared_data->barrier_sem_1); /// this semaphore was initialized with 1, so it will become 0
					sem_post(&args_data->shared_data->barrier_sem_2); /// "free" this semaphore so one thread can pass through it
				}				
				pthread_mutex_unlock(&args_data->shared_data->barrier_mod_mutex); /// we dont need exclusive access anymore
				sem_wait(&args_data->shared_data->barrier_sem_2); /// if the semaphore is 0 the thread will wait until it becomes 1 (the last thread [the one that enters the if statement] will let it become 1)
				sem_post(&args_data->shared_data->barrier_sem_2); /// the thread that has passed on the previous line will increment the semaphore, so the next thread can pass through it, etc.

				/// For restarting the barrier (more specifically the blocked threads counter and the semaphores):
				pthread_mutex_lock(&args_data->shared_data->barrier_mod_mutex); /// get exclusive access to the shared variables again
				args_data->shared_data->blocked_threads_counter--;
				if (args_data->shared_data->blocked_threads_counter == 0) {
					sem_wait(&args_data->shared_data->barrier_sem_2); /// this semaphore ended up with a value of 1, so it will become 0 again (like it was initialized)
					sem_post(&args_data->shared_data->barrier_sem_1); /// "free" this semaphore so one thread can pass through it
				}
				pthread_mutex_unlock(&args_data->shared_data->barrier_mod_mutex); /// we dont need exclusive access anymore
				sem_wait(&args_data->shared_data->barrier_sem_1); /// if the semaphore is 0 the thread will wait until it becomes 1 (the last thread [the one that enters the if statement] will let it become 1)
				sem_post(&args_data->shared_data->barrier_sem_1); /// the thread that has passed will increment the semaphore, so the next thread can pass through it too
				
				/// At the end, both semaphores will have their initial value, so the barrier can be used again.
			break;
			
			case CMD_EMPTY:
				/// do nothing
			break;

			case EOC:
				to_continue = 0; /// stop the while loop
			break;
		}

		line_num++;
	}

	close(args_data->input_fd);
	free(args_data);
	return NULL;
}

int thread_manager_for_file_processing(char* input_filename, int number_of_threads) {
	char* output_filename = filename_extension_changer(input_filename, OUTPUT_EXTENSION); /// get the output filename by changing the extension of the input filename
	
	if (output_filename != NULL) { 
		int output_fd; /// file descriptor for the output file (will be used by all threads)
		if ((output_fd = open(output_filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
			fprintf(stderr, "Error: Unable to create a the file: %s\n", output_filename);
			exit(EXIT_FAILURE);
		}

		pthread_t *threads = malloc(sizeof(pthread_t) * (long unsigned int) number_of_threads); /// allocate memory for the threads
		if (threads == NULL) {
			fprintf(stderr, "Error: Memory allocation for threads failed\n");
			exit(EXIT_FAILURE); 
		}

		thread_shared_data shared_data; /// shared data between threads
		pthread_mutex_init(&shared_data.output_write_mutex, NULL); /// initialize the mutex used to safely write to the output file descriptor
		pthread_mutex_init(&shared_data.events_general_mutex, NULL); /// this mutex is used just for touching the events list (because, in addition to it, each event has its own rwlock)
		sem_init(&shared_data.barrier_sem_1, 0, 1); /// initialize the semaphore 1 for the barrier (explained on the barrier implementation)
		sem_init(&shared_data.barrier_sem_2, 0, 0); /// initialize the semaphore 2 for the barrier (explained on the barrier implementation)
		pthread_mutex_init(&shared_data.barrier_mod_mutex, NULL); /// initialize the mutex for the barrier (explained on the barrier implementation)
		shared_data.blocked_threads_counter = 0;

		int i;
		for (i = 0; i < number_of_threads; i++) {
			thread_args *args= (thread_args*) malloc(sizeof(thread_args)); /// allocate memory for the arguments passed to each thread
			if (args == NULL) {
				fprintf(stderr, "Error: Memory allocation for the thread args failed\n");
				exit(EXIT_FAILURE);
			}

			if ((args->input_fd = open(input_filename, O_RDONLY)) == -1) { /// open a file descriptor for each thread (each thread must close its own fd)
				fprintf(stderr, "Error: Unable to open the file: %s\n", input_filename);
				exit(EXIT_FAILURE);
			}
			
			args->output_fd = output_fd;
			args->shared_data = &shared_data;
			args->number_of_threads = number_of_threads;
			args->thread_id = i + 1; /// the thread id (1..number_of_threads)

			if (pthread_create(&threads[i], NULL, process_file, (void*) args) != 0) { /// if the thread was created successfully
				fprintf(stderr, "Error: Failed to create a thread\n");
				free(args);
			}
		}

		for (i = 0; i < number_of_threads; i++) { /// wait for all created threads to finish
			pthread_join(threads[i], NULL);
		}
		
		if (pthread_mutex_destroy(&shared_data.output_write_mutex) != 0) { /// destroy the mutex used to safely write to the output file descriptor
			fprintf(stderr, "Error: Failed to destroy the output mutex\n");
			exit(EXIT_FAILURE);
		}

		if (pthread_mutex_destroy(&shared_data.events_general_mutex) != 0) { /// destroy the mutex used for updating the barrier state, etc.
			fprintf(stderr, "Error: Failed to destroy the general mutex for events\n");
			exit(EXIT_FAILURE);
		}

		if (pthread_mutex_destroy(&shared_data.barrier_mod_mutex) != 0) { /// destroy the mutex used for updating the barrier state, etc.
			fprintf(stderr, "Error: Failed to destroy the barrier modify mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if (sem_destroy(&shared_data.barrier_sem_1) != 0) { /// destroy one of the semaphores used for the barrier
			fprintf(stderr, "Error: Failed to destroy the first semaphore of the barrier implementation\n");
			exit(EXIT_FAILURE);
		}
		
		if (sem_destroy(&shared_data.barrier_sem_2) != 0) { /// destroy the another semaphore used for the barrier
			fprintf(stderr, "Error: Failed to destroy the second semaphore of the barrier implementation\n");
			exit(EXIT_FAILURE);
		}

		free(threads);
		free(output_filename);
	
	} else {
		fprintf(stderr, "Error: Memory allocation for the output filename failed\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int process_directory_files(const char *dir_path, int number_of_processes, int number_of_threads, unsigned int delay) {
		DIR *dir; /// the specified directory
		struct dirent *entry; 
		int active_processes = 0; /// number of active processes

		if ((dir = opendir(dir_path)) == NULL) {
			printf("Error: Unable to open the directory: %s\n", dir_path);
			exit(EXIT_FAILURE);
		}

		if (chdir(dir_path) != 0) { /// change the directory to the specified directory
			fprintf(stderr, "Error: Unable to change the directory to: %s\n", dir_path);
			exit(EXIT_FAILURE);
		}

		while ((entry = readdir(dir)) != NULL) {
			if (strstr(entry->d_name, EXTENSION_TO_PROCESS)) { /// if the file has the ".jobs" extension
				while (active_processes >= number_of_processes) { /// wait for a process slot to be available
					pid_t child_pid = wait(NULL);

					if (child_pid > 0) { /// if the child process has finished
						active_processes--;
					}
				}

				pid_t pid = fork(); 

				if (pid == -1) { /// if the fork failed
					fprintf(stderr, "Error: Unable to fork\n");
					exit(EXIT_FAILURE);
				} else if (pid == 0) { /// code for the child process
					ems_init(delay);
					thread_manager_for_file_processing(entry->d_name, number_of_threads); /// process the file with threads
					ems_terminate();
					closedir(dir); /// close the directory in the child process
					exit(EXIT_SUCCESS); /// exit the child process
				} else { /// code for the parent process
					active_processes++;
				}
			}
		}

		while (active_processes > 0) { /// wait for all child processes to finish
			int status; /// the status of the child process
			pid_t child_pid = wait(&status);

			if (child_pid > 0) { 
				active_processes--;

				if (WIFEXITED(status)) {
					int exit_status = WEXITSTATUS(status); /// the exit status of the child process

					if (exit_status == EXIT_SUCCESS) {
						printf("The child process %d returned without errors (exit status: %d)\n", child_pid, exit_status);
					} else {
						printf("The child process %d returned with errors (exit status: %d)\n", child_pid, exit_status);
					}
				} else {
						printf("Child process %d terminated atypically\n", child_pid);
				}
			} else {
				fprintf(stderr, "Error: Error while waiting for a child process\n");
				exit(EXIT_FAILURE);
			}
		}

	closedir(dir);
	return 0;
}
