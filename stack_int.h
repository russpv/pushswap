#ifndef STACKINT_H
# define STACKINT_H

#include "stack.h"

#define MAX_PARTITIONS 16
#define INIT_IDX_VALUE -1
#define INIT_NUM_VALUE LONG_MAX

struct	s_stack
{
	char	    		id;
	size_t				max_size;
	size_t				size;
	
	long				*nums; // actual integers, not modified
	int					*idx;  // order index of nums, ops modify this
	int					*part_idx;  // partition id, 1:1 with nums
	t_partition_ptr 	partitions[MAX_PARTITIONS];
};

struct	s_partition
{
	int	id;
	size_t		size; // increment on add
	t_stack_ptr	stack;
};

#endif