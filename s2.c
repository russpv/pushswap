#include "pushswap.h"

// Some version of the greedy search
// The only requirement is that the destination be in stack order

static int abs_min(int a, int b)
{
	int i;
	int j;

	if (a < 0)
		i = -a;
	else
		i = a;
	if (b < 0)
		j = -b;
	else
		j = b;
    if (i < j)
        return (a);
    else if (j < i)
        return (b);
    else
        return (a);
}

/* Positive for rotations; negative for reverse rots */
static int get_depth(t_stack_ptr s, int idx)
{
    if (idx < get_middle_idx(s)) // TODO
        return (idx);
    else 
        return (idx - (get_stack_size(s) - 1));
}

static size_t  count_moves_to_sorted_position(t_stack_ptr s, \
		long num, enum e_move_type m)
{
    int i;
    size_t counter;
    t_stack_ptr c;

    i = 0;
    counter = 0;
    c = copy_stack(s);
	if (!c)
		return (SIZE_MAX);
    push_stack(c, num, create_partition(c));
    while (i < get_middle_idx(c))
    {
        move(s, m);
        counter++;
		i++;
        if (is_sorted_asc(c))
            break ;
    }
    destroy_stack(c);
    return (counter);
}

/* Negative for rotations; positive for reverse rots */
static int get_target_depth(t_stack_ptr s, long num)
{
    size_t rotates;
    size_t reverse_rotates;
    
    rotates = count_moves_to_sorted_position(s, num, ROTATE);
    reverse_rotates = count_moves_to_sorted_position(s, \
			num, REV_ROTATE);

    return (abs_min(rotates, reverse_rotates));
}

/* Returns moves to rotate and push a number to sorted position; absolute value */
static int get_total_moves(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int one = get_depth(s->curr_stack, idx);
    const int two = get_target_depth(s->dest_stack, num);

    if (one >= 0 && two >= 0)
        return (one + two + 1);
    else if (one < 0 && two < 0)
        return (-one + -two + 1);
    else if (one < 0 && two >= 0)
        return (one + two + 1 + abs_min(one, two));
    else if (one >= 0 && two < 0)
        return (one + two + 1 + abs_min(one, two));
    else
        return (INT_MAX);
}

/* Loop each num, return minimum score idx */
static int find_cheapest_idx(t_state *s)
{
    int minimum;
    int i;
	int a;

    i = 0;
    minimum = INT_MAX;
    while (i < (int)get_stack_size(s->curr_stack))
    {
        a = get_total_moves(s, i);
        if (a < minimum)
            minimum = i;
        i++;
    }
    return (minimum);
}

/*******/

// If sign of depth opposite sign of target depth, then rotate both

static bool    do_cheapest_num(t_state *s)
{
    const int   src_idx = find_cheapest_idx(s);
    int	dest_moves = get_target_depth(s->dest_stack, get_stack_num(s->curr_stack, src_idx));
    int	src_moves = get_depth(s->curr_stack, src_idx);

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
        move(s->curr_stack, REV_ROTATE);
        dest_moves++;
    }
    while (src_moves > 0)
    {
        move(s->curr_stack, ROTATE);
        src_moves--;
    }
    move(s->curr_stack, PUSH);
    while (dest_moves < 0)
    {
        move(s->dest_stack, REV_ROTATE);
        dest_moves++;
        // TODO check if push, or would this never happen
    }
    while (dest_moves > 0)
    {
        move(s->dest_stack, ROTATE);
        dest_moves--;
        //TODO check if push
    }
    return (true);
}

bool	greedy_sort(t_state *s)
{
	while (get_stack_size(s->curr_stack) > 0)
		if (do_cheapest_num(s) == false)
			return (false);
	return (true);
}
