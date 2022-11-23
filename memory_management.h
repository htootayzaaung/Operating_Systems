struct memory_block
{
	int is_free;
	size_t size;
	void* address;
	struct memory_block *next
};

struct memory_block *allocate_memory_block(size_t size)
{
	
}

void *_malloc(int size)
/*
	The prototype for malloc should be : void *_malloc(int size)

	The prototype for free should be: void _free(void *ptr)
*/