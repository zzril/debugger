#ifndef BREAKPOINT_H
#define BREAKPOINT_H
// --------

struct breakpoint {
	void* memory_address;
	short active;
	char original_content;
};

// --------

struct breakpoint* breakpoint_new_alloc(void* addr);
struct breakpoint* breakpoint_new_at(struct breakpoint* bp, void* addr);

void breakpoint_destroy_alloc(struct breakpoint* bp);
void breakpoint_destroy_at(struct breakpoint* bp);

void breakpoint_activate(struct breakpoint* bp);
void breakpoint_deactivate(struct breakpoint* bp);

// --------
#endif


