#ifndef STACK_INT_H
# define STACK_INT_H

# include "stack.h"

# define MAX_PARTITIONS 32
# define INIT_IDX_VALUE -1
# define INIT_NUM_VALUE LONG_MAX
# define MAX_SIZE 10000

# define GREY "\033[90m"
# define LTGREY "\033[38;5;245m"
# define RESET "\033[0m"

# define DEBUGGING true

struct				s_stack
{
	char			id;
	size_t			max_size;
	size_t			size;
	long			tmp;
	long			min;
	long			max;
	size_t			last_update_size;

	long			*nums;
	int				*idx;
	int				*part_idx;
	size_t			partition_count;
	t_partition_ptr	partitions[MAX_PARTITIONS];
};

struct				s_partition
{
	int				id;
	size_t			size;
	t_stack_ptr		stack;
};

#endif
