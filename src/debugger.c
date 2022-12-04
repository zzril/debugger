#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <unistd.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "breakpoint.h"
#include "debugger.h"
#include "inject.h"
#include "inspect.h"

// --------

#define MAX_BREAKPOINTS 16

// --------

static char** argv_subprocess = NULL;
static pid_t child_pid = -1;
static void* breakpoint_addresses[MAX_BREAKPOINTS];
static struct breakpoint breakpoints[MAX_BREAKPOINTS];
static int num_breakpoints = 0;

// --------

static char* get_pathname() {
	return argv_subprocess == NULL ? NULL : argv_subprocess[0];
}

static void insert_breakpoints() {
	for(int i = 0; i < num_breakpoints; i++)  {
		breakpoint_new_at(breakpoints + i, breakpoint_addresses[i]);
	}
}

static struct breakpoint* get_breakpoint_at(void* addr) {

	for(int i = 0; i < num_breakpoints; i++)  {

		// First sanitize, then find it in the breakpoints array:
		if(breakpoint_addresses[i] == addr && breakpoints[i].memory_address == addr) {

			return breakpoints + i;
		}
	}
	return NULL;
}

static struct breakpoint* get_current_breakpoint() {
	return get_breakpoint_at((char*) get_rip() - 1);
}

static void start_tracee() {

	puts("Creating subprocess...");

	ptrace_request(PTRACE_TRACEME, NULL, NULL);	// we can use this as `pid` is ignored

	execvp(get_pathname(), argv_subprocess);

	perror("execvp");
	exit(EXIT_FAILURE);
}

static int wait_for_child() {

	int wstatus;

	// Wait for child:
	if(waitpid(child_pid, &wstatus, 0) == -1) { perror("waitpid"); exit(EXIT_FAILURE); }

	// Check if child exited:
	if(WIFEXITED(wstatus)) {
		printf("%s exited with exit code %d.\n", get_pathname(), WEXITSTATUS(wstatus));
		return 1;
	}

	return 0;
}

static void continue_child() {
	ptrace_request(PTRACE_CONT, NULL, NULL);
	return;
}

static int breakpoint_execute_single_step(struct breakpoint* bp) {

	set_rip_relative(-1);

	breakpoint_deactivate(bp);

	ptrace_request(PTRACE_SINGLESTEP, NULL, NULL);

	int child_exited = wait_for_child();

	if(!child_exited) {
		breakpoint_activate(bp);
	}

	return child_exited;
}

static int breakpoint_continue(struct breakpoint* bp) {

	int child_exited = breakpoint_execute_single_step(bp);

	if(!child_exited) {
		continue_child();
	}

	return child_exited;
}

static int continue_any(struct breakpoint* bp) {
	if(bp != NULL) {
		return breakpoint_continue(bp);
	}
	else {
		continue_child();
		return 0;
	}
}

static void run_child_till_exit() {

	// Run till end of execve call:
	if(wait_for_child() != 0) {
		fprintf(stderr, "%s could not be started.\n", get_pathname());
		exit(EXIT_FAILURE);
	}

	// Take the chance to set ptrace options:
	// Kill tracee if tracer exits:
	ptrace_request(PTRACE_SETOPTIONS, NULL, (void*) PTRACE_O_EXITKILL);

	// Insert all the breakpoints:
	insert_breakpoints();

	// Dump registers once:
	printf("\nRegisters at process start:\n");
	print_registers();

	// Run till breakpoints:
	printf("Running %s till breakpoint(s)...\n", get_pathname());
	continue_child();

	int exited = 0;

	while(!exited && wait_for_child() == 0) {

		struct breakpoint* bp = get_current_breakpoint();
		if(bp != NULL) {
			printf("Hit breakpoint at %p.\n", bp->memory_address);
		}
		else {
			printf("Process stopped.\n");
		}
		// Dump registers:
		printf("\nRegisters:\n");
		print_registers();

		puts("Continuing...");
		exited = continue_any(bp);
	}

	exit(EXIT_SUCCESS);
}

// --------

void tracee_set_commandline(char** argv) {
	argv_subprocess = argv;
	return;
}

int tracee_register_breakpoint_at(void* addr) {

	if(num_breakpoints == MAX_BREAKPOINTS) {
		fprintf(stderr, "Too many breakpoints.\n");
		return -1;
	}

	breakpoint_addresses[num_breakpoints] = addr;

	num_breakpoints++;

	return num_breakpoints;
}

long ptrace_request(enum __ptrace_request request, void* addr, void* data) {

	errno = 0;

	long return_value = ptrace(request, child_pid, addr, data);

	if(errno != 0) {
		perror("ptrace");
		exit(EXIT_FAILURE);
	}

	return return_value;
}

int debugger_run() {

	child_pid = fork();

	switch(child_pid) {
		case 0:
			start_tracee();
		case -1:
			perror("fork"); exit(EXIT_FAILURE);
		default:
			run_child_till_exit();
	}

	fputs("main: Control flow unexpectedly reached end of function.\n", stderr);
	exit(EXIT_FAILURE);
}


