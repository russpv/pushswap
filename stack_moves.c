#include "stack_int.h"

/* Moves the top element to the last */
// Debug statement to add:

bool	rotate_stack(t_stack_ptr stack)
{
	int	top_num_idx;
	int	top_part_id;

	if (!stack || !stack->idx)
		return (false);
	if (stack->size <= 1)
		return (true);
	top_num_idx = stack->idx[0];
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int));
	stack->idx[stack->size - 1] = top_num_idx;
	top_part_id = stack->part_idx[0];
	ft_memmove(&stack->part_idx[0], &stack->part_idx[1], (stack->size - 1)
		* sizeof(int));
	stack->part_idx[stack->size - 1] = top_part_id;
	if (true == DEBUGGING)
		mydebug("Log: Rotating %d on s:%d(%d) p:%d\n", get_stack_num(stack, \
		top_num_idx), stack->id, top_num_idx, top_part_id);
	return (true);
}

/* Moves the last element to the top */
// Debug statement to add:

bool	rev_rotate_stack(t_stack_ptr stack)
{
	int	last_num_idx;
	int	last_part_id;

	if (!stack || !stack->idx)
		return (false);
	if (stack->size <= 1)
		return (false);
	last_num_idx = stack->idx[stack->size - 1];
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->size - 1) * sizeof(int));
	stack->idx[0] = last_num_idx;
	last_part_id = stack->part_idx[stack->size - 1];
	ft_memmove(&stack->part_idx[1], &stack->part_idx[0], (stack->size - 1)
		* sizeof(int));
	stack->part_idx[0] = last_part_id;
	if (true == DEBUGGING)
		mydebug("Log: Reversing %d on s:%d(%d) p:%d\n", get_stack_num(stack, \
		last_num_idx), stack->id, last_num_idx, last_part_id);
	return (true);
}

bool	swap_stack(t_stack_ptr stack)
{
	int	second_num_idx;

	if (!stack || !stack->idx)
	{
		return (false);
	}
	if (stack->size <= 1)
	{
		return (false);
	}
	second_num_idx = stack->idx[1];
	stack->idx[1] = stack->idx[0];
	stack->idx[0] = second_num_idx;
	return (true);
}
