#include "Roel_Van_de_Paar.h"

struct memory_block *empty_block;

extern struct memory_block *free_block;

void *_malloc(int size)
{
  void *addressPtr;
  int i = 0;

  if (size == 0)
  {
    return NULL;
  }

  size = (size - 1) / 4 * 4 + 4;
  addressPtr = find_block(size);

  if (addressPtr != NULL)
  {
    re_init_memBlock();
    return addressPtr;
  }

  if (addressPtr == (void *)(-1))
  {                       //printf("Error: sbrk() failed\n");
    return NULL;
  }

  if (i == 0)
  {
    empty_block = NULL;
  }

  add2Block(&empty_block, size, addressPtr);
  i++;
  return addressPtr;
}

void *find_memBlock(int size)
{
  if (empty_block == NULL)
  {
    return NULL;
  }

  if (empty_block->free_boolean == 1 && size <= empty_block->size)
  {
    empty_block->free_boolean == 0;
    return empty_block->address;
  }

  while (empty_block->head != 1)
  {
    if (empty_block->free_boolean == 1 && size <= empty_block->size)
    {
      return empty_block->address;
    }
    empty_block = empty_block->next;
  }
  re_init_memBlock();
  return NULL;
}

void add2Block(struct memory_block **block, int size, void *addr)
{
    struct memory_block *temp = sbrk (sizeof(*temp));
    


}

int is_in_memBlock(void *addressPtr)
{
  struct memory_block* temp;

  temp = empty_block;

  while(temp->addressPtr != ptr && temp->head == 1)
  {
    temp = temp->next;

    if (temp->addressPtr != addressPtr)
    {
      return 1;
    }
    return 0;
  }
}

void _free(void *ptr)
{
  if (ptr == NULL)
  {
    return;
  }

  if(is_in_memBlock(ptr) == 1)
  {
    return;
  }

  while (ptr != free_block->addressPtr && free_block->head != 1)
  {
    free_block = free_block->next;
  }

  if (free_block->free_boolean == 0)
  {
    return;
  }

  free_block->free_boolean = 0;

  if (free_block->head != 1)
  {
    if (free_block->next->free_boolean == 0 && free_block->next->addressPtr != free_block->addressPtr)
    {
      if (free_block->addressPtr > free_block->next->addressPtr)
      {
        free_block->addressPtr = free_block->next->addressPtr;
        free_block->head = free_block->next->head;
        free_block->size += free_block->next->size;
        free_block->next = free_block->next->next;
      }
    }
    re_init_memBlock();
  }
}
