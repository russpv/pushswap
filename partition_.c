#include "stack_int.h"

/* Called only by create_partition() */
static int	_get_next_free_partition_id(t_stack_ptr stack)
{
	int	i;

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
	t_partition_ptr	partition;

	partition = malloc(sizeof(struct s_partition));
	if (partition)
	{
		partition->id = _get_next_free_partition_id(stack);
		partition->size = 0;
		partition->stack = stack;
		if (-1 == partition->id)
		{
			mydebug("\033[31mERR partition create failure\n\033[0m");
			return (free(partition), NULL);
		}
		stack->partitions[partition->id] = partition;
		stack->partition_count++;
	}
	mydebug("---- (create_part): Init'd part, id:%d stack:%d\n", partition->id,
		stack->id);
	return (partition);
}

t_partition_ptr	copy_partition(t_partition_ptr src, t_stack_ptr dest_stack)
{
	t_partition_ptr	partition;

	partition = malloc(sizeof(struct s_partition));
	if (partition)
	{
		partition->id = src->id;
		partition->size = src->size;
		partition->stack = dest_stack;
		dest_stack->partitions[partition->id] = partition;
		dest_stack->partition_count++;
		mydebug("---- (copy_part): copied part, id:%d stack:%d\n",
			partition->id, dest_stack->id);
	}
	return (partition);
}

void	destroy_partition(t_partition_ptr *p)
{
	free(*p);
	*p = NULL;
}

/* Modifies stack partition id array */
bool	fill_partition(t_stack_ptr stack, t_partition_ptr partition, int begin,
		int end)
{
	int	i;

	if (!stack || !partition || begin < 0 || end < begin
		|| end >= (int)stack->size)
		return (false);
	i = begin;
	mydebug("---- (fill_part): Init partition fill id:%d\n", partition->id);
	while (i <= end)
	{
		stack->part_idx[i] = partition->id;
		partition->size++;
		i++;
	}
	return (true);
}
