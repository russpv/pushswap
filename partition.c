#include "stack_int.h"

/* Called only by create_partition() */
static int	_get_next_free_partition_id(t_stack_ptr stack)
{
	int i;

	i = -1;
	while (++i < MAX_PARTITIONS)
		if (stack->partitions[i] == NULL)
			break ;
	if (i == MAX_PARTITIONS)
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
		if (-1 == partition->id)
			return (free(partition), NULL);
		stack->partitions[partition->id] = partition;
        stack->partition_count++;
	}
	mydebug("Log: Init'd part, id:%d stack:%d\n", partition->id, stack->id);
	return (partition);
}

t_partition_ptr copy_partition(t_partition_ptr src, t_stack_ptr dest_stack)
{
    t_partition_ptr partition = malloc(sizeof(struct s_partition));
    if (partition)
    {
        partition->id = src->id;
        partition->size = src->size;
        partition->stack = dest_stack;
        dest_stack->partitions[partition->id] = partition;
        dest_stack->partition_count++;
        mydebug("Log: copied part, id:%d stack:%d\n", partition->id, dest_stack->id);
    }
    return (partition);
}

void    destroy_partition(t_partition_ptr *p)
{
    free(*p);
    *p = NULL;
}

/* Modifies stack partition id array */
bool	fill_partition(t_stack_ptr stack, 
						t_partition_ptr	partition, 
						int begin, 
						int end)
{
	int i;

	if (!stack || !partition || begin < 0 || end < begin || end >= (int)stack->size)
		return (false);
	i = begin;
	mydebug( "Log: Init partition fill id:%d\n", partition->id);
	while (i <= end)
	{
		stack->part_idx[i] = partition->id;
		partition->size++;
		i++;
	}
	return (true);
}
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
    {
        s->partitions[idx]->size--;
        s->partition_count--;
        destroy_partition(&s->partitions[idx]);
    }
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
		mydebug( "Log: median:%ld\n", arr[size / 2]);
		return (arr[size / 2]);
	}
	mydebug( "Log: median:%ld\n", arr[size / 2 -1 ]);
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
	if (NULL == stack)
	{
		mydebug( "hey stack is NULLLL"); fflush(stderr);
		return (NULL);
	}
	const int part_id = stack->part_idx[0];
	int i;
	i = -1;
	if (INIT_IDX_VALUE == part_id)
		mydebug( "bad top num part_id\n");
	mydebug( "Log: top num part_id:%d\n", part_id); fflush(stderr);
	while (++i < MAX_PARTITIONS)
		if (stack->partitions[i] && part_id == stack->partitions[i]->id)
			return (stack->partitions[i]);
	if (i >= MAX_PARTITIONS)
		return (NULL);
	return (stack->partitions[i]);
}

int	get_partition_id(t_partition_ptr p)
{
	return (p->id);
}

size_t  get_partition_count(t_stack_ptr s)
{
    return (s->partition_count);
}

long	peek_partition(t_partition_ptr p)
{
	return (peek_stack(p->stack));
}
