#include <stdio.h>
#include <stdlib.h>

#include "breakpoint.h"
#include "inject.h"
#include "inspect.h"

// --------

#define INT3 '\xCC'

// --------

static void breakpoint_init(struct breakpoint* bp, void* addr) {
	bp->memory_address = addr;
	bp->active = 0;
	bp->original_content = read_memory_byte(addr);
	return;
}

// --------

struct breakpoint* breakpoint_new_alloc(void* addr) {
	struct breakpoint* bp = malloc(sizeof(struct breakpoint));
	if(bp == NULL) { perror("malloc"); return NULL; }
	return breakpoint_new_at(bp, addr);
}

struct breakpoint* breakpoint_new_at(struct breakpoint* bp, void* addr) {
	breakpoint_init(bp, addr);
	breakpoint_activate(bp);
	return bp;
}

void breakpoint_destroy_alloc(struct breakpoint* bp) {
	breakpoint_destroy_at(bp);
	free(bp);
}

void breakpoint_destroy_at(struct breakpoint* bp) {
	if(bp->active) {
		breakpoint_deactivate(bp);
	}
	return;
}

void breakpoint_activate(struct breakpoint* bp) {
	write_memory_byte(bp->memory_address, INT3);
	bp->active = 1;
	return;
}

void breakpoint_deactivate(struct breakpoint* bp) {
	write_memory_byte(bp->memory_address, bp->original_content);
	bp->active = 0;
	return;
}


