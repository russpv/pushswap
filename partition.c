#include "stack.h"
#include "stack.c"

bool    increment_partition(t_stack_ptr s, int idx)
{
    if (NULL == s->partitions[idx])
        return (false);
    s->partitions[idx]->size++;
    return (true);
}

bool    decrement_partition(t_stack_ptr s, int idx)
{
    if (NULL == s->partitions[idx])
        return (false);
    if (1 == s->partitions[idx]->size)
        destroy_partition(s->partitions[idx]);
    else
        s->partitions[idx]->size--;
    return (true);
}

/* Returns median of numbers on top of stack */
int		get_median(long *nums, int *idx, size_t size)
{
	int flag;
	long tmp;
	size_t i;
	long arr[size];

	i = 0;
	while (i < size)
	{
		arr[i] = nums[idx[i]];
		i++;
	}
	if (size == 1)
		return (arr[0]);
	while (1)
	{
		i = 0;
		flag = 0;
		while (++i < size) 
		{
			if (arr[i] < arr[i - 1]) // sort ascending rightward
			{
				tmp = arr[i];
				arr[i] = arr[i - 1];
				arr[i - 1] = tmp;
				flag = 1;
			}
		}
		if (flag == 0)
		   break ;	
	}
	//_print_arr(arr, size);
	if (size % 2) //odd
	{
		fflush(stderr);
		fprintf(stderr, "median:%ld|", arr[size / 2]);
		return (arr[size / 2]);
	}
	fprintf(stderr, "median:%ld|", arr[size / 2 -1 ]);
	return (arr[size / 2 - 1]);
}

int	get_top_partition_median(t_stack_ptr stack)
{
	const t_partition_ptr	partition = get_top_partition(stack);

	return (get_median(stack->nums, stack->idx, partition->size));
}

size_t	get_partition_size(t_partition_ptr partition)
{
	return (partition->size);
}
/* Returns ptr to topmost partition on stack */
t_partition_ptr get_top_partition(t_stack_ptr stack)
{
	const int part_id = stack->part_idx[0];
	int i;

	i = 0;
	while (i < MAX_PARTITIONS)
		if (part_id != stack->partitions[i]->id)
			i++;
	if (i >= MAX_PARTITIONS)
		return (NULL);
	return (stack->partitions[i]);
}

static int	_get_next_free_partition_id(t_stack_ptr stack)
{
	int i;

	i = -1;
	while (++i < MAX_PARTITIONS)
		if (stack->partitions[i] == NULL)
			break ;
	if (0 == i)
		i = -1;
	return (i);
}

/* Creates new empty partition on stack */
t_partition_ptr	create_partition(t_stack_ptr stack)
{
	t_partition_ptr	partition = malloc(sizeof(struct s_partition));
	if (partition)
	{
		partition->id = _get_next_free_partition_id(stack);
		partition->size = 0;
		partition->stack = stack;
		//partition->idx = malloc(sizeof(int) * stack->max_size);
		if (-1 == partition->id)
			return (free(partition), NULL);
		stack->partitions[partition->id] = partition;
	}
	return (partition);
}

void    destroy_partition(t_partition_ptr p)
{
    free(p);
}

/* DEPRECATED
 * void	fill_partition(t_stack_ptr stack, 
						t_partition_ptr	partition, 
						int begin, 
						int end)
{
	int i;

	i = begin;
	while (i <= end)
	{
		partition[i] = stack[i];
		partition->size++;
	}
}*/

