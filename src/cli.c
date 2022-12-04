#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>

#include "debugger.h"

// --------

struct resources {
	char* tracee_pathname;
	char** tracee_argv;
};

// --------

static void print_usage_msg(FILE* stream, char** argv) {
	fprintf(stream, "Usage: %s [-b breakpoints] pathname [arguments]\n", argv[0]);
	return;
}

static void fail_with_usage_msg(char** argv) {
	print_usage_msg(stderr, argv);
	exit(EXIT_FAILURE);
}

static void parse_breakpoints(char* breakpoints) {

	char* bp_current;
	char* start_ptr = breakpoints;
	char* endptr;

	while((bp_current = strtok(start_ptr, ",")) != NULL) {

		// Parse to long:
		errno = 0;
		long bp_value = strtol(bp_current, &endptr, 0);
		if(errno != 0) {
			perror("strtol");
			exit(EXIT_FAILURE);
		}
		if(*endptr != '\0') {
			fprintf(stderr, "Invalid breakpoint.\n");
			exit(EXIT_FAILURE);
		}

		if(tracee_register_breakpoint_at((void*) bp_value) < 0) {
			fprintf(stderr, "Could not set breakpoint.\n");
			exit(EXIT_FAILURE);
		}

		// Continue in same string:
		start_ptr = NULL;
	}

	return;
}

static void parse_args(int argc, char** argv) {

	if(argc < 2) {
		fail_with_usage_msg(argv);
	}

	int tracee_argv_offset = 1;

	if(strcmp(argv[1], "-b") == 0) {
		tracee_argv_offset += 2;
		parse_breakpoints(argv[2]);
	}

	tracee_set_commandline(argv + tracee_argv_offset);

	return;
}

// --------

int main(int argc, char** argv) {

	parse_args(argc, argv);

	debugger_run(argc, argv);

	exit(EXIT_SUCCESS);
}


