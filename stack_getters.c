#include "stack_int.h"

int	get_stack_id(t_stack_ptr s)
{
	if (!s)
		return (-1);
	return (s->id);
}

size_t	get_stack_size(t_stack_ptr s)
{
	if (!s)
		return (SIZE_MAX);
	return (s->size);
}

long	get_stack_num(t_stack_ptr s, int idx)
{
	if (!s || idx < 0)
		return (LONG_MAX);
	if (idx >= (int)get_stack_size(s))
		return (LONG_MAX);
	return (s->nums[s->idx[idx]]);
}

size_t	get_stack_max_size(t_stack_ptr s)
{
	if (!s)
		return (SIZE_MAX);
	return (s->max_size);
}

/* Accounts for wrap-around i
 * depth: -ve or +ve
 */
int	get_stack_idx_at_depth(t_stack_ptr s, int depth)
{
	if (s->size < 2)
		return (INT_MAX);
	while (depth < 0)
		depth += s->size;
	return (depth % s->size);
}
