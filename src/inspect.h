#ifndef INSPECT_H
#define INSPECT_H
// --------

#define EFLAGS_CF 1 << 0x0
#define EFLAGS_PF 1 << 0x2
#define EFLAGS_AF 1 << 0x4
#define EFLAGS_ZF 1 << 0x6
#define EFLAGS_SF 1 << 0x7
#define EFLAGS_OF 1 << 0xb

// --------


long read_memory_long(void* addr);
char read_memory_byte(void* addr);

void* get_rip();

void print_memory_long(void* addr);
void print_memory_byte(void* addr);

void print_registers();
void print_registers_at(void* regs);

// --------
#endif


