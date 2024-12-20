#include "stack_int.h"

/* Returns ptr to topmost partition on stack */
t_partition_ptr	get_top_partition(t_stack_ptr stack)
{
	const int	part_id = stack->part_idx[0];
	int			i;

	if (NULL == stack)
	{
		mydebug("ERR hey stack is NULLLL");
		fflush(stderr);
		return (NULL);
	}
	i = -1;
	if (INIT_IDX_VALUE == part_id)
		mydebug("ERR bad top num part_id\n");
	while (++i < MAX_PARTITIONS)
		if (stack->partitions[i] && part_id == stack->partitions[i]->id)
			return (stack->partitions[i]);
	if (i >= MAX_PARTITIONS)
		return (NULL);
	return (stack->partitions[i]);
}

size_t	get_partition_size(t_partition_ptr partition)
{
	return (partition->size);
}

int	get_partition_id(t_partition_ptr p)
{
	return (p->id);
}

size_t	get_partition_count(t_stack_ptr s)
{
	return (s->partition_count);
}

int	get_top_partition_median(t_stack_ptr stack)
{
	const t_partition_ptr	partition = get_top_partition(stack);

	return (get_median(stack->nums, stack->idx, partition->size));
}
