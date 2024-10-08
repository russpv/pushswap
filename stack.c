#include "stack.h"

#define MAX_PARTITIONS 16

struct	s_stack
{
	const char			id;
	size_t				max_size;
	size_t				size;
	
	long				*nums; // actual integers, not modified
	int					*idx;  // order index of nums, ops modify this
	int					*part_idx;  // partition id, 1:1 with nums
	struct s_partition	*partitions[MAX_PARTITIONS];
};

struct	s_partition
{
	const int	id;
	size_t		size; // increment on add
	t_stack_ptr	stack;
}

t_stack_ptr	create_stack(const char id, const size_t size)
{
	int i;

	t_stack_ptr stack = malloc(sizeof(s_stack));
	if (stack)
	{
		stack->id = id;
		stack->max_size = size;
		stack->nums = malloc(sizeof(long) * size);
		if (!stack->nums)
			return (free(stack), NULL);
		stack->idx = malloc(sizeof(int) * size);
		if (!stack->idx)
			return (free(stack->nums), free(stack), NULL);
		i = 0;
		while (i < MAX_PARTITIONS)
			stack->partitions[i++] = NULL;
	}
	return (stack);
}

void	destroy_stack(t_stack_ptr stack)
{
	int i;

	i = -1;
	while (++i < MAX_PARTITIONS)
	{
		if (stack->partitions[i] != NULL)
			destroy_partition(stack->partitions[i]);
		stack->partitions[i] = NULL;
	}
	free(stack->idx);
	free(stack->nums);
	free(stack);
}

void	fill_stack(t_stack_ptr stack)
{
	//TODO
	stack->size++;
}

/* Returns top value */
long	peek_top(t_stack_ptr stack)
{
	return (stack->nums[stack->idx[0]]);
}

void	pop_stack(t_stack_ptr stack)
{
	const long value = stack->nums[stack->idx[0]];
	stack->nums[stack->idx[0]] = LONG_MAX;
	// overwrite 
	stack->idx
	// overwrite
	stack->part_idx
}

void	push_stack(t_stack_ptr stack, int num, struct s_partition *partition)
{
	const int idx = get_next_free_idx(stack->nums);
	stack->nums[idx] = num;
	// moves down idx
	stack->idx[0] = idx;
	// move downb stack->part_idx[];
	stack->part_idx[0] = partition->id;
}

/* Moves the top element to the last */
void	rotate_stack(t_stack_ptr stack)
{
	int top_num_idx;

	if (!stack || !stack->idx)
		exit(EXIT_FAILURE);
	if (stack->size <= 1)
		return ;
	top_num_idx = stack->idx[0];
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int));
   	stack->idx[stack->size - 1] = top_num_idx;
	write(1, "ra\n", 3); // TODO print the stack id
}

/* Moves the last element to the top */
void	rev_rotate_stack(t_stack_ptr stack)
{
	int last_num_idx;

	if (!stack || !stack->idx)
		exit(EXIT_FAILURE);
	if (stack->size <= 1)
		return ;
	last_num_idx = stack->idx[stack->size - 1];
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->size - 1) * sizeof(int));
   	stack->idx[0] = last_num_idx;
	write(1, "rra\n", 3); // TODO print the stack id
}

void	rotate_both(t_state *s)
{
	(void)s;
}

void	rev_rotate_both(t_state *s)
{
	(void)s;
}
