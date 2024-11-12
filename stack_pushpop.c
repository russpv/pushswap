#include "stack_int.h"

static int	_get_next_free_idx(t_stack_ptr stack)
{
	int	i;

	i = -1;
	while (++i < (int)stack->max_size)
		if (stack->idx[i] == INIT_IDX_VALUE)
			return (i);
	if ((int)stack->max_size == i)
		i = -1;
	return (i);
}

/* Adds num to top of stack assuming stack not full
 * And there are still free partition indices if 
 * valid partition_ptr is not passed
 */
bool	push_stack(t_stack_ptr stack, int num, t_partition_ptr partition)
{
	const int	free_idx = _get_next_free_idx(stack);

	if (-1 == free_idx)
		return (false);
	if (NULL == partition)
		partition = create_partition(stack);
	if (true == DEBUGGING)
		mydebug("Log: Pushing %d on s:%d(i:%d) p:%d\n", num, stack->id, \
		free_idx, partition->id);
	stack->nums[free_idx] = num;
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->max_size - 1)
		* sizeof(int));
	stack->idx[0] = free_idx;
	ft_memmove(&stack->part_idx[1], &stack->part_idx[0], (stack->max_size - 1)
		* sizeof(int));
	stack->part_idx[0] = partition->id;
	stack->size++;
	if (false == increment_partition(stack, partition->id))
		return (false);
	return (true);
}

/* Removes top value from num array, idx array, and partition
 * Decrements partition and size
 * Note: arg checks should go first but style guide prohibits
 */
long	pop_stack(t_stack_ptr stack)
{
	const long				top_value = stack->nums[stack->idx[0]];
	const t_partition_ptr	top_partition = get_top_partition(stack);

	if (NULL == stack || 0 == stack->size)
		return (LONG_MAX);
	if (true == DEBUGGING)
		mydebug("Log: Popping %d on s:%d(i:%d) p:%d\n", top_value, \
		stack->id, 0, top_partition->id);
	stack->nums[stack->idx[0]] = LONG_MAX;
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int));
	ft_memmove(&stack->part_idx[0], &stack->part_idx[1], (stack->size - 1) \
		* sizeof(int));
	stack->idx[(int)stack->size - 1] = INIT_IDX_VALUE;
	stack->part_idx[(int)stack->size - 1] = INIT_IDX_VALUE;
	if (false == decrement_partition(stack, top_partition->id))
		return (LONG_MAX);
	stack->size--;
	return (top_value);
}
