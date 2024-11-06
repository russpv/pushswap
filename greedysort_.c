#include "pushswap.h"

static bool	_do_more_rotates(t_state *s, int dest_moves)
{
	while (dest_moves < 0)
	{
		move(s->dest_stack, REV_ROTATE, 0, NULL, PRINT_ON);
		dest_moves++;
	}
	while (dest_moves > 0)
	{
		move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
		dest_moves--;
	}
	return (true);
}

static bool	_do_rotates(t_state *s, int dest_moves, int src_moves)
{
	while (dest_moves < 0 && src_moves < 0)
	{
		rev_rotate_both(s);
		dest_moves++;
		src_moves++;
	}
	while (dest_moves > 0 && src_moves > 0)
	{
		rotate_both(s);
		dest_moves--;
		src_moves--;
	}
	while (src_moves < 0)
	{
		move(s->curr_stack, REV_ROTATE, 0, NULL, PRINT_ON);
		src_moves++;
	}
	while (src_moves > 0)
	{
		move(s->curr_stack, ROTATE, 0, NULL, PRINT_ON);
		src_moves--;
	}
	if (false == _do_more_rotates(s, dest_moves))
		return (false);
	return (true);
}

/* If sign of depth opposite sign of target depth, then rotate both
 * Interprets negative moves as reverse rotates
 * Does not rotate back to starting position after push
 */
static bool	_do_cheapest_num(t_state *s)
{
	int				dest_moves;
	int				src_moves;
	t_partition_ptr	partition;

	mylog("NEXT (do_cheapest_num) -- size %zu num %ld\n",
		get_stack_size(s->curr_stack), peek_stack(s->curr_stack));
	if (!is_sorted_asc_rotationally(s->dest_stack))
		err("ERR: stack sorted incorrectly\n", s);
	if (find_cheapest_idx(s) == INT_MAX)
		return (false);
	dest_moves = s->target_depth;
	src_moves = s->depth;
	if (get_partition_count(s->dest_stack) == 0)
		partition = create_partition(s->dest_stack);
	else
		partition = get_top_partition(s->dest_stack);
	if (false == _do_rotates(s, dest_moves, src_moves))
		return (false);
	move(s->dest_stack, PUSH, pop_stack(s->curr_stack), partition, PRINT_ON);
	mylog("(do_cheapest_num) -- Finished. \n");
	return (true);
}

static bool	_rotate_back_to_head(t_stack_ptr s)
{
	int	moves;

	moves = get_moves_back_to_head(s);
	mydebug("Moves back got:%d\n", moves);
	if (moves > 0)
		while (moves--)
			move(s, ROTATE, 0, NULL, PRINT_ON);
	else if (moves < 0)
		while (moves++ < 0)
			move(s, REV_ROTATE, 0, NULL, PRINT_ON);
	if (is_sorted_asc(s))
		return (true);
	return (false);
}

bool	greedy_sort(t_state *s)
{
	size_t	i;

	i = get_stack_size(s->curr_stack);
	mylog("\033[45mGREEDY SORT *********************\n\033[0m");
	mylog("\033[45mGREEDY SORT *********************\n\033[0m");
	mylog("\033[45mGREEDY SORT *********************\n\033[0m");
	while (i-- > 0)
		if (_do_cheapest_num(s) == false)
			return (false);
	if (!_rotate_back_to_head(s->dest_stack))
		err("ERR rotate_back problem\n", s);
	print_stacks(s);
	mylog("Greedy sort is done.\n");
	return (true);
}
