#include "memory_management.h"

static struct mem_Chain *start = 0;

void delete(struct mem_Chain *chain)
{
    if (!(chain->prev)) 
    {
        if (chain->next)      
        {
            start = chain->next;
        }
        else
        {
            start = 0;
        }
    }
    else
    {
        chain->prev->next = chain->next;
    }
    if (chain->next)
    {
        chain->next->prev = chain->prev;
    }
}

void add(struct mem_Chain *chain)
{
    chain->prev = 0;
    chain->next = 0;

    if (!(start) || start > chain)
    {
        if (start)
        {
            start->prev = chain;
        }
        chain->next = start;
        start = chain;
    }
    else
    {
        struct mem_Chain *temp = start;

        while(temp->next && temp->next < start)
        {
            temp = temp->next;
        }
        chain->next = temp->next;
        temp->next = chain;
    }
}

void search_coalesce_setBreak()
{
    struct mem_Chain *temp = start;

    unsigned long program_break = (unsigned long) sbrk(0);
    unsigned long currentAddr, nextAddr;

    if (program_break == 0)
    {
        printf("Program break cannot be found!");
        return;
    }

    while (temp->next)
    {
        currentAddr = (unsigned long)temp; 
        nextAddr = (unsigned long)temp->next;

        if (currentAddr + temp->size + sizeof(struct mem_Chain) == nextAddr)
        {
            temp->size += temp->next->size + sizeof(struct mem_Chain);
            temp->next = temp->next->next;
            
            if (temp->next)
            {
                temp->next->prev = temp;
            }
            else
            {
                break;
            }
        }
        temp = temp->next;
    }

    currentAddr = (unsigned long)temp;

    if (currentAddr + temp->size + sizeof(struct mem_Chain) == program_break)
    {
        delete(temp);
    }
}

struct mem_Chain *partition(struct mem_Chain *chain, int size)
{
    void *mem_block = MEM_BLOCK(chain);
    struct mem_Chain *temp = (struct mem_Chain*)((unsigned long)mem_block + sizeof(struct mem_Chain));
    temp->size = chain->size - (size + sizeof(struct mem_Chain));
    chain->size = size;
    return temp;
}

void *_malloc(int size)
{
    void *mem_block;
    struct mem_Chain *start_clone, *temp;

    int allocation = size + 4096;
    start_clone = start;
    
    while (start_clone) 
    {
        if (start_clone->size >= size + sizeof(struct mem_Chain)) 
        {
            mem_block = MEM_BLOCK(start_clone);
            delete(start_clone);
            if (start_clone->size == size) 
            {
                return mem_block;
            }

            temp = partition(start_clone, size);
            add(temp);
            return mem_block;
        } else 
        {
            start_clone = start_clone->next;
        }
    }

    start_clone =(struct mem_Chain*) sbrk(allocation);
    
    if (!start_clone) 
    {
        printf("failed to alloc %ld\n", allocation);
        return 0;
    }

    start_clone->next = 0;
    start_clone->prev = 0;
    start_clone->size = allocation - sizeof(struct mem_Chain);

    if (allocation > size + sizeof(struct mem_Chain)) 
    {
        temp = partition(start_clone, size);
        add(temp);
    }
    
    return MEM_BLOCK(start_clone);
}

void _free(void *ptr)
{

}

