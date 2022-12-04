#include <stdlib.h>

#include <sys/user.h>

#include "debugger.h"
#include "inject.h"
#include "inspect.h"

// --------

void write_memory_long(void* addr, long data) {
	ptrace_request(PTRACE_POKETEXT, addr, (void*) data);
	return;
}

//TODO: make Endianness-independent
void write_memory_byte(void* addr, char data) {

	long byte_mask = 0xff;
	long rest_mask = ~byte_mask;

	long old_data = read_memory_long(addr);

	long rest = old_data & rest_mask;
	long new_data_masked = (long) data & byte_mask;	// making sure the typecast doesn't destroy the higher bits
	long new_data_word = new_data_masked | rest;

	write_memory_long(addr, new_data_word);

	return;
}

void set_rip(void* addr) {

	struct user_regs_struct regs;

	ptrace_request(PTRACE_GETREGS, NULL, &regs);

	regs.rip = (long) addr;

	ptrace_request(PTRACE_SETREGS, NULL, &regs);

	return;
}

void set_rip_relative(long offset) {

	struct user_regs_struct regs;

	ptrace_request(PTRACE_GETREGS, NULL, &regs);

	regs.rip += offset;

	ptrace_request(PTRACE_SETREGS, NULL, &regs);

	return;
}


