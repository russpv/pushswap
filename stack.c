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

t_stack_ptr	create_stack(const char id, const size_t size)
{
	size_t i;

	t_stack_ptr stack = malloc(sizeof(struct s_stack));
	if (stack)
	{
		stack->id = id;
		stack->max_size = size;
		stack->nums = malloc(sizeof(long) * size);
		stack->idx = malloc(sizeof(int) * size);
        stack->part_idx = malloc(sizeof(int) * size);
		if (!stack->nums || !stack->idx || !stack->part_idx)
            return (free(stack->nums), free(stack->idx), free(stack->part_idx), NULL);
        i = 0;
        while (i < size)
        {
            if (i < MAX_PARTITIONS)
			    stack->partitions[i] = NULL;
            stack->nums[i] = INIT_NUM_VALUE;
            stack->idx[i] = INIT_IDX_VALUE;
            stack->part_idx[i++] = INIT_IDX_VALUE; 
        }
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
    free(stack->part_idx);
	free(stack->nums);
	free(stack);
}

/* Populates nums and idx only */
void	fill_stack(t_stack_ptr stack, char **argv)
{
    size_t i;

    i = 0;
    if (NULL == stack)
        return ;
	while (argv[i] != NULL)
    {
        stack->nums[i] = (long)ft_atoi(argv[i]);
        stack->idx[i] = i;
        stack->size++;
        i++;;
    }
}

/* Returns top value */
long	peek_top(t_stack_ptr stack)
{
	return (stack->nums[stack->idx[0]]);
}

/* Removes top value from num array, idx array, and partition */
long	pop_stack(t_stack_ptr stack)
{
	const long  top_value = stack->nums[stack->idx[0]];
    const int   top_part_idx = stack->part_idx[0];

    stack->nums[stack->idx[0]] = LONG_MAX;
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->max_size - 1) * sizeof(int)); 
	ft_memmove(&stack->part_idx[0], &stack->part_idx[1], (stack->max_size - 1) * sizeof(int)); 
	decrement_partition(stack, top_part_idx);
    return (top_value);
}

static int	_get_next_free_idx(t_stack_ptr stack)
{
	size_t i;

	i = 0;
	while (i < stack->max_size)
    {
		if (stack->idx[i] == INIT_IDX_VALUE)
			break ;
        i++;
    }
	if (0 == i)
		i = -1;
	return (i);
}

/* Adds num to top of stack assuming stack not full and partition has been created */
bool	push_stack(t_stack_ptr stack, int num, t_partition_ptr partition)
{
	const int idx = _get_next_free_idx(stack);
	
    if (NULL == partition)
        return (false);
    stack->nums[idx] = num;
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->max_size - 1) * sizeof(int));
	stack->idx[0] = idx;
	ft_memmove(&stack->part_idx[1], &stack->part_idx[0], (stack->max_size - 1) * sizeof(int));
	stack->part_idx[0] = partition->id;
    if (false == increment_partition(stack, partition->id))
        return (false);
    return (true);
}

/* Moves the top element to the last */
bool    rotate_stack(t_stack_ptr stack)
{
	int top_num_idx;

	if (!stack || !stack->idx)
		return (false);
	if (stack->size <= 1)
		return (true);
	top_num_idx = stack->idx[0];
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int));
   	stack->idx[stack->size - 1] = top_num_idx;
    return (true);
}

/* Moves the last element to the top */
bool    rev_rotate_stack(t_stack_ptr stack)
{
	int last_num_idx;

	if (!stack || !stack->idx)
        return (false);
	if (stack->size <= 1)
		return (true);
	last_num_idx = stack->idx[stack->size - 1];
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->size - 1) * sizeof(int));
   	stack->idx[0] = last_num_idx;
    return (true);
}

bool    is_sorted_asc(t_stack_ptr s)
{
	size_t i;
	const int *idx = s->idx;
    const long    *nums = s->nums; 

	i = 1;
	if (s->size < (size_t)2)
		return (true);
	while (i < s->size)
	{
		if (nums[idx[i - 1]] > nums[idx[i]])
			return (false);
		i++;
	}
	return (true);
}

void print_stack_id(t_stack_ptr stack)
{
    write(1, (const char*)&stack->id, 1);
    write(1, "\n", 1);
}

void	print_stack(t_stack_ptr stack)
{
	const size_t size = stack->size;
	if (0 == size)
		return ;
	for (size_t j = 0; j < size - 1; j++)
		fprintf(stderr, " %ld", stack->nums[stack->idx[j]]);
	fprintf(stderr,"|");
}

