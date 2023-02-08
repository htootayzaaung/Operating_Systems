#ifndef MEMORY_MANAGEMENT
#define MEMORY_MANAGEMENT

#include "kernel/types.h"
#include "user/user.h"

/*
    If a pointer this structure has the pointers "next" and "prev" that are null, "mem_Chain" behaves as if it's a single block.
    Otherwise, it behaves as a list of memory-blocks linked together as a linked-list.
*/

struct mem_Chain
{
    int size;
    struct mem_Chain *next;
    struct mem_Chain *prev;
};

#define MEM_BLOCK(ptr)((void *)((unsigned long)ptr + sizeof(struct mem_Chain)))

#define MEM_START(ptr)((void *)((unsigned long)ptr - sizeof(struct mem_Chain)))

void add(struct mem_Chain *chain);

void delete(struct mem_Chain *chain);

void search_coalesce_setBreak();

struct mem_Chain *partition(struct mem_Chain *chain, int size);

void *_malloc(int size);

void _free(void *ptr);

#endif