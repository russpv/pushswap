#include "pushswap.h"

/* Positive for rotations; negative for reverse rots */
static int	_get_depth(t_stack_ptr s, int idx)
{
	int	result;

	if (get_stack_size(s) == 1)
		result = 0;
	else if (idx <= get_middle_idx(s))
		result = idx;
	else
		result = (idx - get_stack_size(s));
	mydebug("-- (get_depth) -- for num:%ld (size:%zu, idx:%d) moves:%d\n", \
		get_stack_num(s, idx), get_stack_size(s), idx, result);
	return (result);
}

/* Returns +ve or -ve moves to correct insertion position in s
 * Translates insertion position into rotates and reverse rotates
 * Returns max number if position is beyond middle index
 */
static void	_count_moves_to_sorted_position(t_stack_ptr s, long num,
		int *rotates, int *reverse_rotates)
{
	int	pos;

	pos = get_insert_position(s, num);
	if (pos > (int)get_middle_idx(s))
	{
		*rotates = INT_MAX;
		*reverse_rotates = pos - (int)get_stack_size(s);
	}
	else
	{
		*rotates = pos;
		*reverse_rotates = INT_MAX;
	}
	mydebug("---- (count moves) -- pos:%d midx:%d rots:%d rrotes:%d\n", pos, \
		get_middle_idx(s), *rotates, *reverse_rotates);
}

/* Returns -ve or +ve moves to put num in sorted position in s
 * negative for reverse rotations; positive for normal rotations
 * Case 1: stack too small for moves, returns 0
 * Case 2: moves are beyond midpoint, return opposite moves
 */
static int	_get_target_depth(t_stack_ptr s, long num)
{
	int	rotates;
	int	reverse_rotates;

	if (get_stack_size(s) < 2)
		return (0);
	_count_moves_to_sorted_position(s, num, &rotates, &reverse_rotates);
	mydebug("-- (get_target_depth) -- for num %ld (size:%zu): target depth: %d, \
		rev_rots:%d rots:%d\n", num, get_stack_size(s), abs_min(rotates, \
			reverse_rotates), reverse_rotates, rotates);
	if (rotates == INT_MAX)
		return (reverse_rotates);
	return (rotates);
}

/* Returns total moves to rotate and push a number to sorted 
 * position; absolute value
 * If either both reverse and normal rotations involved,
	takes the absolute value maximum
 * (since double rots are possible
 */
int	get_moves(const int one, const int two)
{
	if (one >= 0 && two >= 0)
		return (myabs(abs_max(one, two)) + 1);
	else if (one < 0 && two < 0)
		return (myabs(abs_max(one, two)) + 1);
	else if (one < 0 && two >= 0)
		return (myabs(one) + two + 1);
	else if (one >= 0 && two < 0)
		return (one + myabs(two) + 1);
	else
		return (INT_MAX);
}

/* Saves 'depth' or moves for idx num into s
 */
int	get_total_moves(t_state *s, int idx)
{
	const long	num = get_stack_num(s->curr_stack, idx);
	const int	one = _get_depth(s->curr_stack, idx);
	const int	two = _get_target_depth(s->dest_stack, num);

	if (one == INT_MAX || two == INT_MAX || num == LONG_MAX)
		return (INT_MAX);
	s->depth = one;
	s->target_depth = two;
	s->moves = get_moves(one, two);
	mydebug("-- (get_total_moves) -- idx:%d num:%d depth:%d \
		dest_depth:%d (push 1)\n",
		idx, num, one, two);
	return (s->moves);
}
