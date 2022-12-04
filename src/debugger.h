#ifndef DEBUGGER_H
#define DEBUGGER_H
// --------

#include <sys/ptrace.h>

// --------

void tracee_set_commandline(char** argv);

int tracee_register_breakpoint_at(void* addr);

long ptrace_request(enum __ptrace_request request, void* addr, void* data);

int debugger_run();

// --------
#endif


