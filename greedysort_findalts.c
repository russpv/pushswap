#include "pushswap.h"

long	distance(long a, long b)
{
	return (myabs(a - b));
}

static int	_increment(int i, int direction)
{
	if (direction < 0)
		return (--i);
	else
		return (++i);
}

/* Returns index of better number to push.
 * If a push candidate's destination stack insertion position 
 * ("depth") in terms of
 * moves is absolutely higher than the moves it takes to bring 
 * it to the top of current stack
 * for the push, this looks for alternative "closer" candidates 
 * using "free" double
 * moves. If t_depth is -ive, increment negatively
 * using get_stack_idx_at_depth allows index to wrap around.
 * 
 * Arguments:
 * idx: This is the idx on currstack for the lowest move num
 * i: offset from idx, +ve (rotates) or -ve (rev rots)
 * min_dist: difference to beat 
 * 
 * CHANGES: uses struct tmp vars instead of local
 */
// Debug statements to add back:
	/* mydebug("\033[34m---- (find alts) -- nonzero target depth,\
		testing destnum:%ld t_depth:%d, vs num:%ld idx:%d depth:%d\
		 dist:%ld\033[0m\n", dest_num, s->target_depth, num, idx, \
		 s->depth, min_dist); */
	// mydebug("\033[44m---- (Find alts) -- closer:%d\n\033[0m", s->tmp);
	/* mydebug("\033[34m---- (Find alts) -- returning num:%ld idx:%d 
	depth:%d (iter:%d)\n\033[0m",
		get_stack_num(s->curr_stack, new_idx), new_idx, s->depth, i); */
int	find_alts(t_state *s, int idx)
{
	const long	num = get_stack_num(s->curr_stack, idx);
	const long	dest_num = get_stack_num(s->dest_stack, \
		get_stack_idx_at_depth(s->dest_stack, s->target_depth));
	long		min_dist;
	int			i;

	min_dist = distance(dest_num, num);
	i = 0;
	if (s->target_depth == 0 || myabs(s->target_depth) <= idx
		|| dest_num == LONG_MAX || distance(dest_num, num) == 1)
		return (idx);
	while (myabs(i) < myabs(s->target_depth))
	{
		i = _increment(i, s->target_depth);
		s->tmp = get_stack_num(s->curr_stack, \
			get_stack_idx_at_depth(s->curr_stack, i + idx));
		if (s->tmp == LONG_MAX)
			break ;
		if ((s->tmp < dest_num) && distance(s->tmp, dest_num) < min_dist)
		{
			s->depth = get_stack_idx_at_depth(s->curr_stack, i + idx);
			min_dist = distance(s->tmp, dest_num);
		}
	}
	return (s->depth);
}
