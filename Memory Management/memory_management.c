#include "memory_management.h"

/*
 start
 _____ ____ _____
|*prev|size|*next|      Initialized as a pointer to a structure that initially points nowhere.
|_____|____|_____|

_Global-pointer variable that will be used to stored items that during the free process and other memory-fragments.
*/

static struct mem_Chain *start = 0;     

//Adds memory-blocks to the linked-list

void add(struct mem_Chain *chain)
{
    //Assigns NULL-pointers by default
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
        struct mem_Chain *temp = start;     //Copy of a global-pointer variable

        while(temp->next && temp->next < start)
        {
            temp = temp->next;              //iterates through temp
        }
        chain->next = temp->next;
        temp->next = chain;
    }
}

/*
    deletes a memory-block or memory-chain from a supplied pointer-variable to the structure
    re-adjusting the leading-node.
*/
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

/*
    Searches vacant blocks so that it can be coalesced and validates if the ending memory-block terminates
    where the program break is. 
*/

void search_coalesce_setBreak()
{
    struct mem_Chain *temp = start;         //Copy of a global-pointer variable

    unsigned long program_break = (unsigned long) sbrk(0);
    unsigned long currentAddr, nextAddr;

    if (program_break == 0)
    {
        return;
    }

    while (temp->next)
    {
        currentAddr = (unsigned long)temp; 
        nextAddr = (unsigned long)temp->next;

        if (currentAddr + temp->size + sizeof(struct mem_Chain) == nextAddr)
        //Checking the conditions where there are adjacent free blocks and merging them
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
    /*
        Checking if the last memory-block on the free list terminates where program break begins,
        and discarded if the size is appropriate (i.e to limit the number of sbrk() function calls.)
    */
    {
        delete(temp);
    }
}

/*
    Splits "chain" into a smaller memory block whose block is a reduce size of "size" of its original size.
*/
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
    if (size == 0)
    {
        return 0;
    }
    
    void *mem_block;
    struct mem_Chain *start_clone, *temp;

    int allocation = size + 4096;   //size_allocated + typical_page_size of 4096 bytes
    start_clone = start;
    
    /*
        It scans the list of memory blocks previously released by free() in order to find one whoose size is larger
        than or equal to the reuqirements.
    */
    while (start_clone) 
    {
        if (start_clone->size >= size + sizeof(struct mem_Chain)) 
        {
            mem_block = MEM_BLOCK(start_clone);
            delete(start_clone);
            if (start_clone->size == size)      //requested-memory-block equivalent to one of freed-memory blocks
            {
                return mem_block;
            }
            /*
                requested-memory-block greater than those on a linked-list that stores freed-memory blocks
                if such eventuality occurs partitioning takes place.
            */
            temp = partition(start_clone, size);
            add(temp);
            return mem_block;
        } 
        
        else //If non-of those conditions are met it iterates through the linked-list
        {
            start_clone = start_clone->next;
        }
    }

    /*
        The only eventuality when OS should facilitate sbrk() function call is when the system has requested
        more memory-blocks than size of the addresses that are stored in the memory-chain that contains all
        previousy freed memory-blocks.
    */
    start_clone =(struct mem_Chain*) sbrk(allocation);
    
    if (!start_clone) 
    {
        return 0;
    }

    if (start_clone == (void*) - 1)
    {
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
    if (!ptr || ptr > (void *)sbrk(0))  //Check if the input-pointer is outside of the heap.
    {
        return;
    }

    //adds a memory-block to a free-list
    add(MEM_START(ptr));
    search_coalesce_setBreak();
}


// int main(int argc, char *argv[])
// {
//     int *intArr; char *charArr; 
//     int n = 5; 

//     intArr = (int*)_malloc(n * sizeof(int));
//     //charArr = (char*)_malloc(n * sizeof(char));
    
//     //MALLOC test for integer-array
//     for (int i = 0; i < n; i++) 
//     {
//         intArr[i] = i + 1;
//     }

//     printf("Integers: \n");

//     for (int i = 0; i < n; i++) 
//     {
//         printf("%d->%p\n", intArr[i], &intArr[i]);
//     }

//     //MALLOC test for character-array
//     printf("\nCharatcers: \n");

//     for (int i = 0; i < n; i++)
//     {
//         charArr[i] = (char)(65 + i);
//         printf("%c->%p\n", charArr[i], &charArr[i]);
//     }

//     //TEST FOR FREE
//     /*_free(&intArr[0]);

//     printf("\n%d->%p", intArr[0], &intArr[0]);
//     printf("\nintArr[0] = %d\n\n", intArr[0]);

//     int *intPtr = (int *)_malloc(sizeof(int)); 
//     intPtr = (int *) 2;
//     printf("Before freeing:\nintPtr->%p\n\n", intPtr);
    
//     _free(intPtr);
//     printf("\nAfter freeing:\nintPtr->%p\n", intPtr);*/
    
//     //TEST FOR MEMORY LEAK

//     /*int *memoryLeak;
//     int leak = 99999999; //really large leak causes memory-leak
//     memoryLeak = (int *)_malloc(leak * sizeof(int));

//     for (int i = 0; i < leak; i++) 
//     {
//         memoryLeak[i] = i + 1;
//     }

//     printf("\nMemory-leak: \n");

//     for (int i = 0; i < leak; i++) 
//     {
//         printf("%d->%p\n", memoryLeak[i], &memoryLeak[i]);
//     }*/
    

//     return 0;
// }

