#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "constants.h"
#include "parser.h"
#include "operations.h"
#include "processing/processing.h"

typedef struct Data Data;

int main(int argc, char *argv[]) {
	unsigned int state_access_delay_ms = STATE_ACCESS_DELAY_MS; /// default delay

	if (argc == 5) {  // if the delay is specified
		char *endptr;
		unsigned long int delay = strtoul(argv[4], &endptr, 10);

		if (delay > UINT_MAX) { // if the delay is too large
			fprintf(stderr, "Error: Invalid delay value or value too large\n");
			return 1;
		} 

		state_access_delay_ms = (unsigned int) delay; // set the delay to the specified value
	}

	if (argc == 4 || argc == 5) { // if the correct number of arguments are passed
		if (process_directory_files(argv[1], atoi(argv[2]), atoi(argv[3]), state_access_delay_ms) != 0) { /// process the directory files
			fprintf(stderr, "Error: Failed to process the directory files.\n"); 
			return 1;
		}
	} else { // if the incorrect number of arguments are passed
		fprintf(stderr, "Error: Incorrect number of arguments.\n");
		fprintf(stderr, "Usage: %s <directory> <number of processes> <number of threads> [delay in ms]\n", argv[0]);
		return 1;
	} 
}
