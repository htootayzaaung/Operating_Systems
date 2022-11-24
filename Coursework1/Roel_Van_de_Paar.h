/*
  https://www.youtube.com/watch?v=HUXS5mo738w
*/

#ifndef ROEL_VAN_DE_PAAR
#define ROEL_VAN_DE_PAAR

#include <unistd.h>
#include <string.h>
#include <stdio.h>

struct memory_block
{
  int size;
  int free_boolean;             //boolean-1 corresponds to true and boolean-0 corresponds to false
  void *address;
  int head;
  struct memory_block *prev;
  struct memory_block *next;
};

void *_malloc(int size);

void *find_memBlock(int size);

void add2Block(struct memory_block *block, int size, void *addressPtr);    //void put_in_list(t_list **list, size_t size, void *addr);

void re_init_memBlock();  //void re_init_list();

void _free(void *ptr);

int is_in_memBlock(void *ptr);  //returns 1 when not in the memory_block, returns 0 for success

void link_head_tail(struct memory_block **block);

#endif
