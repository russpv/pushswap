#include "stack_int.h"

bool	is_full(t_stack_ptr s)
{
	if (!s)
		return (false);
	return (s->size == s->max_size);
}

/* Returns true if both are the same */
bool	stack_compare(int a, t_stack_ptr b)
{
	if (a == b->id)
		return (true);
	return (false);
}

/* Returns true for empty stack */
bool	is_sorted_asc(t_stack_ptr s)
{
	size_t		i;
	const int	*idx = s->idx;
	const long	*nums = s->nums;

	i = 1;
	if (!s)
		return (false);
	if (s->size == 0 || s->size == 1)
		return (true);
	while (i < s->size)
	{
		if (nums[idx[i - 1]] > nums[idx[i]])
			return (false);
		i++;
	}
	if (true == DEBUGGING)
		mydebug("---- (is_sorted_asc): SORTED!\n");
	return (true);
}

/* Written assuming a single disjoint 'drop' in the sequence 
 * Returns true for empty stack
 */
bool	is_sorted_asc_rotationally(t_stack_ptr s)
{
	int	drops;
	int	i;

	drops = 0;
	i = -1;
	if (!s)
		return (false);
	while (++i < (int)s->size)
		if (s->nums[s->idx[i]] > s->nums[s->idx[(i + 1) % s->size]])
			drops++;
	if (drops > 1)
	{
		if (true == DEBUGGING)
			mydebug("---- (is sorted rotationally) -- not sorted.\n");
		return (false);
	}
	else
	{
		if (true == DEBUGGING)
			mydebug("---- (is sorted rotationally) -- SORTED!!\n");
		return (true);
	}
}
