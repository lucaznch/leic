#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "api.h"
#include "common/constants.h"
#include "parser.h"

void sigpipe_handler(int signum) {
	(void) signum;
	const char* msg = "We received a SIGPIPE signal. Maybe the server disconnected?\n";
	write(STDERR_FILENO, msg, strlen(msg));
	exit(1);
}

int main(int argc, char* argv[]) {
	if (signal(SIGPIPE, sigpipe_handler) == SIG_ERR) {
		fprintf(stderr, "Failed to set SIGPIPE handler\n");
		return 1;
	}

	if (argc < 5) { // 4 arguments + 1 for the program name
		fprintf(stderr, "Usage: %s <request pipe path> <response pipe path> <server pipe path> <.jobs file path>\n", argv[0]);
		return 1;
	}

	const char* dot = strrchr(argv[4], '.'); // Checks if the file path is valid
	if (dot == NULL || dot == argv[4] || strlen(dot) != 5 || strcmp(dot, ".jobs") ||
			strlen(argv[4]) > MAX_JOB_FILE_NAME_SIZE) {
		fprintf(stderr, "The provided .jobs file path is not valid. Path: %s\n", argv[1]);
		return 1;
	}

	char out_path[MAX_JOB_FILE_NAME_SIZE];
	strcpy(out_path, argv[4]);
	strcpy(strrchr(out_path, '.'), ".out");

	int in_fd = open(argv[4], O_RDONLY); // opens the input file
	if (in_fd == -1) {
		fprintf(stderr, "Failed to open input file. Path: %s\n", argv[4]);
		return 1;
	}

	int out_fd = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0666); // opens the output file
	if (out_fd == -1) {
		fprintf(stderr, "Failed to open output file. Path: %s\n", out_path);
		return 1;
	}
	
	ems_setup_data setup_info = ems_setup(argv[1], argv[2], argv[3]); // setup the client

	if (setup_info.return_code == 1) {
		fprintf(stderr, "Failed to set up the client\n");
		return 1;
	}

	printf("The server received our request. Our session ID is %d\n", setup_info.session_id);

	while (1) {
		unsigned int event_id;
		size_t num_rows, num_columns, num_coords;
		unsigned int delay = 0;
		size_t xs[MAX_RESERVATION_SIZE], ys[MAX_RESERVATION_SIZE];

		int current_cmd = get_next(in_fd); // Gets the current_cmd command

		switch (current_cmd) {
			case CMD_CREATE:
				if (parse_create(in_fd, &event_id, &num_rows, &num_columns) != 0) { // Parses the create command
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_create(event_id, num_rows, num_columns, setup_info.req_fd, setup_info.resp_fd) != 0) {
					fprintf(stderr, "Failed to create event\n");
				}

				break;

			case CMD_RESERVE:
				num_coords = parse_reserve(in_fd, MAX_RESERVATION_SIZE, &event_id, xs, ys);

				if (num_coords == 0) {
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_reserve(event_id, num_coords, xs, ys, setup_info.req_fd, setup_info.resp_fd) != 0) {
					fprintf(stderr, "Failed to reserve seats\n");
				}

				break;

			case CMD_SHOW:
				if (parse_show(in_fd, &event_id) != 0) {
					fprintf(stderr, "Invalid command. See HELP for usage\n");
					continue;
				}

				if (ems_show(out_fd, event_id, setup_info.req_fd, setup_info.resp_fd) != 0) {
					fprintf(stderr, "Failed to show event\n");
				}
				
				break;

			case CMD_LIST_EVENTS:
				if (ems_list_events(out_fd, setup_info.req_fd, setup_info.resp_fd) != 0) {
					fprintf(stderr, "Failed to list events\n");
				}

				break;

			case CMD_WAIT:
				if (parse_wait(in_fd, &delay, NULL) == -1) {
						fprintf(stderr, "Invalid command. See HELP for usage\n");
						continue;
				}

				if (delay > 0) {
						printf("Waiting...\n");
						sleep(delay);
				}

				break;

			case CMD_INVALID:
				fprintf(stderr, "Invalid command. See HELP for usage\n");
				
				break;

			case CMD_HELP:
				printf(
						"Available commands:\n"
						"  CREATE <event_id> <num_rows> <num_columns>\n"
						"  RESERVE <event_id> [(<x1>,<y1>) (<x2>,<y2>) ...]\n"
						"  SHOW <event_id>\n"
						"  LIST\n"
						"  WAIT <delay_ms>\n"
						"  HELP\n");

				break;

			case CMD_EMPTY:
				break;

			case EOC:
				int return_code;
				if ((return_code = ems_quit(setup_info.req_fd, setup_info.resp_fd, argv[1], argv[2])) != 0) {
					fprintf(stderr, "Failed to quit\n");
				}
				close(in_fd);
				close(out_fd);
				return return_code;
		}
	}
}