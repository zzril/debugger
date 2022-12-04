#include <stdint.h>
#include <stdio.h>

#include <sys/ptrace.h>
#include <sys/user.h>

#include "debugger.h"
#include "inspect.h"

// --------

static void print_single_flag_at(uint64_t* flags, uint64_t flag_mask, const char* name_on, const char* name_off) {
	printf(" %s", *flags & flag_mask ? name_on : name_off);
}

static void print_flags_at(void* data) {

	struct user_regs_struct user_regs = *((struct user_regs_struct*) data);
	uint64_t* flags = (uint64_t*) &(user_regs.eflags);

	printf("Flags:");
	print_single_flag_at(flags, EFLAGS_CF, "CY", "NC");
	print_single_flag_at(flags, EFLAGS_PF, "PE", "PO");
	print_single_flag_at(flags, EFLAGS_AF, "AC", "NA");
	print_single_flag_at(flags, EFLAGS_ZF, "ZR", "NZ");
	print_single_flag_at(flags, EFLAGS_SF, "NG", "PL");
	print_single_flag_at(flags, EFLAGS_OF, "OV", "NV");
	puts("");

	return;
}

// --------

long read_memory_long(void* addr) {
	return ptrace_request(PTRACE_PEEKTEXT, addr, NULL);
}

//TODO: make Endianness-independent
char read_memory_byte(void* addr) {
	return (char) read_memory_long(addr);
}

void* get_rip() {
	struct user_regs_struct regs;
	ptrace_request(PTRACE_GETREGS, NULL, &regs);
	return (void*) (regs.rip);
}

void print_memory_long(void* addr) {
	printf("0x%lx", read_memory_long(addr));
	return;
}

void print_memory_byte(void* addr) {
	printf("%hhx", read_memory_byte(addr));
	return;
}

void print_registers() {

	struct user_regs_struct user_regs;

	ptrace_request(PTRACE_GETREGS, NULL, &user_regs);

	print_registers_at(&user_regs);

	return;
}

void print_registers_at(void* regs) {

	struct user_regs_struct user_regs = *((struct user_regs_struct*) regs);

	puts("");
	printf("rax:   0x%llx\n", user_regs.rax);
	printf("rcx:   0x%llx\n", user_regs.rcx);
	printf("rdx:   0x%llx\n", user_regs.rdx);
	printf("rbx:   0x%llx\n", user_regs.rbx);
	puts("");
	printf("rsp:   0x%llx\n", user_regs.rsp);
	printf("rbp:   0x%llx\n", user_regs.rbp);
	puts("");
	printf("rsi:   0x%llx\n", user_regs.rsi);
	printf("rdi:   0x%llx\n", user_regs.rdi);
	puts("");
	printf("r8 :   0x%llx\n", user_regs.r8);
	printf("r9 :   0x%llx\n", user_regs.r9);
	printf("r10:   0x%llx\n", user_regs.r10);
	printf("r11:   0x%llx\n", user_regs.r11);
	puts("");
	printf("r12:   0x%llx\n", user_regs.r12);
	printf("r13:   0x%llx\n", user_regs.r13);
	printf("r14:   0x%llx\n", user_regs.r14);
	printf("r15:   0x%llx\n", user_regs.r15);
	puts("");
	print_flags_at(regs);
	printf("rip:   0x%llx\n", user_regs.rip);
	puts("");

	return;
}


