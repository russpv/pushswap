#include "pushswap.h"

/* For tiebreaking: Employs extrema check and biases for 
 * numbers closer to deststack number
 * Assumes both have equal moves-to-sorted-position
 * Returns true if curr_idx is closer than ref_idx.
 * ref_idx must be valid
 * Case 1: receives an invalid reference, must return true
 * Case 2: current index moves are less than ref, return true
 * Case 3: current index ties with ref, but is extremum (top or bottom),
	return false
 * Case 4: current index ties with ref, not extremum, and smaller diff,
	return true
 */
// Debug statements to add back:
/* mydebug("\033[32m---- (closer_than) >> checking extremum (%ld destnum\
		%d moves, %d depth, %d t_depth)...\n\033[0m", dest_num, ref_moves, \
		s->depth, s->target_depth); */
/* mydebug("\033[32m---- (closer_than) >> extremum,\
		not closer.\n\033[0m"); */
/* 		mydebug("\033[32m---- (closer_than) >> closer number! (%d - %d) v (%d -\
			%d)\n\033[0m", curr_num, dest_num, ref_num, dest_num);*/
static bool	_closer_than(t_state *s, int curr_idx, int ref_idx, int ref_moves)
{
	const int	dest_idx = get_stack_idx_at_depth(s->dest_stack, \
				s->target_depth);
	const long	dest_num = get_stack_num(s->dest_stack, dest_idx);
	const long	curr_num = get_stack_num(s->curr_stack, curr_idx);
	const long	ref_num = get_stack_num(s->curr_stack, ref_idx);

	if (dest_idx == INT_MAX)
		return (false);
	if (ref_idx == -1)
		return (true);
	if (dest_num == LONG_MAX || curr_num == LONG_MAX || ref_num == LONG_MAX)
		return (false);
	if (get_moves(s->depth, s->target_depth) < ref_moves)
		return (true);
	else if (get_moves(s->depth, s->target_depth) > ref_moves)
		return (false);
	if (is_stack_extremum(s->dest_stack,
			curr_num) ^ is_stack_extremum(s->dest_stack, ref_num))
		if (is_stack_extremum(s->dest_stack, curr_num)
			&& !(is_stack_extremum(s->dest_stack, ref_num)))
			return (false);
	if (distance(curr_num, dest_num) < distance(ref_num, dest_num))
		return (true);
	return (false);
}

/* Returns iterator that is skipped ahead if a 'shallow' move already found AND
 * a number of indices equal to the best move have already been tested from the
 * top of the stack.
 * Move is shallow if total moves is less than or equal to 2 or 10% of N 
 * whichever greater
 * (No sense testing on current stack size of less than 20% of N or 2
 * 	* interval)
 * The skip amount is the best move count away from the bottom of the stack
 */
static int	_skip_pointless_indices(t_state *s, int min_idx_moves, const int i)
{
	const size_t	curr_stack_size = get_stack_size(s->curr_stack);
	const size_t	dest_stack_size = get_stack_size(s->dest_stack);
	int				new_i;

	new_i = i;
	if (3 * curr_stack_size < dest_stack_size)
		return (new_i);
	if (min_idx_moves <= abs_max(s->nums / 10, 2) && i >= min_idx_moves)
	{
		new_i = abs_max(curr_stack_size - min_idx_moves, i);
		if (new_i != i)
			mydebug("\033[47m-- (skip_indices) -- skip to idx:%d (csz:%zu dsz:%zu, \
				min_moves:%d) \n\033[0m", new_i, curr_stack_size, \
				dest_stack_size, min_idx_moves);
	}
	return (new_i);
}

/* Returns index of current stack number that gives the lowest moves
 * Unless a closer number exists for that number's sorted position in
 * destination stack for the same amount of moves.
 * Skips over numbers in middle of stack where sensible
 */
// Debug statements to add back:
/* mydebug("\033[33m-- (find_cheapest) -- result: new min (%ld  i:%d) for \
				%d moves\n\033[0m", get_stack_num(s->curr_stack, i), i, moves); 
				*/
/* mydebug("\033[43m-- (find cheapest) -- Searched %d idx's,\
		Found cheapest i:%d n:%ld moves:%d d:%d td:%d \033[0m\n", i, s->tmp_i, \
		get_stack_num(s->curr_stack, s->tmp_i), s->tmp_moves, s->depth, \
		s->target_depth); */
int	find_cheapest_idx(t_state *s)
{
	int	i;
	int	moves;

	i = 0;
	s->tmp_i = -1;
	while (i < (int)get_stack_size(s->curr_stack))
	{
		moves = get_total_moves(s, i);
		if (moves == 1)
			return (i);
		if (_closer_than(s, i, s->tmp_i, s->tmp_moves) == true)
		{
			s->tmp_i = i;
			s->tmp_moves = s->moves;
			s->tmp_depth = s->depth;
			s->tmp_t_depth = s->target_depth;
		}
		i = _skip_pointless_indices(s, s->tmp_moves, i) + 1;
	}
	s->depth = s->tmp_depth;
	s->target_depth = s->tmp_t_depth;
	if (s->target_depth > 0 && get_stack_size(s->curr_stack) > 1)
		s->tmp_i = find_alts(s, s->tmp_i);
	return (s->tmp_i);
}
