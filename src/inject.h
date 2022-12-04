#ifndef INJECT_H
#define INJECT_H
// --------

void write_memory_long(void* addr, long data);
void write_memory_byte(void* addr, char data);

void set_rip(void* addr);
void set_rip_relative(long offset);

// --------
#endif


