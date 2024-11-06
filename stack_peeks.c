#include "stack_int.h"

/* Returns top value */
long	peek_stack(t_stack_ptr stack)
{
	if (get_stack_size(stack) == 0)
	{
		return (LONG_MAX);
	}
	return (stack->nums[stack->idx[0]]);
}

long	peek_bottom(t_stack_ptr stack)
{
	if (get_stack_size(stack) == 0)
	{
		return (LONG_MAX);
	}
	return (stack->nums[stack->idx[get_stack_size(stack) - 1]]);
}

long	peek_next_stack(t_stack_ptr stack)
{
	if (get_stack_size(stack) < 2)
	{
		return (LONG_MAX);
	}
	return (stack->nums[stack->idx[1]]);
}
