#include "scratch.h"

#define NUM_SCRATCH 7
int scratch_table[NUM_SCRATCH] = {0, 0, 0, 0, 0, 0, 0};

int scratch_alloc()
{
	int i = 0;
	
	// loop through each scratch reg
	for (i = 0; i < NUM_SCRATCH; i++)
	{
		// if it has not been allocated, allocate it and return it
		if (!scratch_table[i]) 
		{
			scratch_table[i] = 1;
			return i;
		}
	}

	// error
	fprintf(stderr, "codegen error: ran out of scratch registers\n");
	return -1;
}

void scratch_free(int r)
{
	// mark it as free
	scratch_table[r] = 0;
}

const char *scratch_name(int r)
{
	if 		(r == 0)		return strdup("%rbx");
	else if 	(r == 1)		return strdup("%r10");
	else if 	(r == 2)		return strdup("%r11");
	else if 	(r == 3)		return strdup("%r12");
	else if 	(r == 4)		return strdup("%r13");
	else if 	(r == 5)		return strdup("%r14");
	else if 	(r == 6)		return strdup("%r15");
	else						return NULL;
}
