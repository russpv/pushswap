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
    if (idx < get_middle_idx(s))
    {
        mydebug("Idx:%d (depth) less that middle idx:%d", idx, get_middle_idx(s));
        return (idx);
    }
    else 
    {
        mydebug("Greater than middle idx, returning :%d", idx- (get_stack_size(s) -1 ));
        return (idx - (get_stack_size(s) - 1));
    }
}

/* Expects s to be the destination stack */
static size_t  count_moves_to_sorted_position(t_stack_ptr s, \
		long num, enum e_move_type m)
{
    int i;
    size_t counter;
    t_stack_ptr c; /* dest stack */

    mydebug("counting moves to sorted posiiton...\n");
    i = 0;
    counter = 0;
    c = copy_stack(s);
	if (!c)
		return (SIZE_MAX);
    push_stack(c, num, create_partition(c));
    while (i < get_middle_idx(c))
    {
        move(s, m, 0, NULL, PRINT_OFF);
        counter++;
		i++;
        if (is_sorted_asc(c))
            break ;
    }
    destroy_stack(c);
    mydebug("Counted moves:%d\n", counter);
    return (counter);
}

/* Negative for rotations; positive for reverse rots */
static int get_target_depth(t_stack_ptr s, long num)
{
    size_t rotates;
    size_t reverse_rotates;
    
    mydebug("Getting target depth..\n");
    rotates = count_moves_to_sorted_position(s, num, ROTATE);
    reverse_rotates = count_moves_to_sorted_position(s, \
			num, REV_ROTATE);
    if (rotates == SIZE_MAX || reverse_rotates == SIZE_MAX)
        return (INT_MAX);
    mydebug("Got target depth: %d\n", abs_min(rotates, reverse_rotates));
    return (abs_min(rotates, reverse_rotates));
}

/* Returns moves to rotate and push a number to sorted position; absolute value */
static int get_total_moves(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int one = get_depth(s->curr_stack, idx);
    const int two = get_target_depth(s->dest_stack, num);

    mydebug("Idx:%d num:%d depth:%d dest_depth:%d\n", idx, num, one, two);
    if (one == INT_MAX || two == INT_MAX)
        return (INT_MAX);
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

/* Loop each num in current stack, return minimum score idx */
static int find_cheapest_idx(t_state *s)
{
    int minimum;
    int i;
	int a;

    mydebug("Finding cheapest idx\n");
    i = 0;
    minimum = INT_MAX;
    while (i < (int)get_stack_size(s->curr_stack))
    {
        a = get_total_moves(s, i);
        mydebug("Total moves for idx:%d\n", a);
        mydebug("----next idx----\n");
        if (a == INT_MAX)
            return (INT_MAX);
        if (a < minimum)
            minimum = i;
        i++;
    }
    mydebug("Found cheapest idx:%d\n", minimum);
    return (minimum);
}

/*******/

// If sign of depth opposite sign of target depth, then rotate both

static bool    do_cheapest_num(t_state *s)
{
    const int   src_idx = find_cheapest_idx(s);
    int	dest_moves = get_target_depth(s->dest_stack, get_stack_num(s->curr_stack, src_idx));
    int	src_moves = get_depth(s->curr_stack, src_idx);
    t_partition_ptr partition;

    if (get_partition_count(s->dest_stack) == 0)
        partition = create_partition(s->dest_stack);
    else
        partition = get_top_partition(s->dest_stack);

    mylog("do cheapest idx:%d num:%lu depth:%d tar_d:%d\n", src_idx, get_stack_num(s->curr_stack, src_idx), \
            src_moves, dest_moves);
    fprintf(stderr, "wrf\n");
    while (dest_moves < 0 && src_moves < 0)
    {
		rev_rotate_both(s);
        dest_moves++;
        src_moves++;
    }
    fprintf(stderr, "heyo\n");
    while (dest_moves > 0 && src_moves > 0)
    {
        rotate_both(s);
        dest_moves--;
        src_moves--;
    }
    fprintf(stderr, "heyo2\n");
    while (src_moves < 0)
    {
        move(s->curr_stack, REV_ROTATE, 0, NULL, PRINT_ON);
        src_moves++;
    }
    fprintf(stderr, "heyo3\n");
    while (src_moves > 0)
    {
        move(s->curr_stack, ROTATE, 0, NULL, PRINT_ON);
        src_moves--;
    }
    fprintf(stderr, "heyo4\n");
    move(s->dest_stack, PUSH, pop_stack(s->curr_stack), partition, PRINT_ON);
    print_stack(s->dest_stack);
    while (dest_moves < 0)
    {
        move(s->dest_stack, REV_ROTATE, 0, NULL, PRINT_ON);
        dest_moves++;
        // TODO check if push, or would this never happen
    }
    while (dest_moves > 0)
    {
        move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
        dest_moves--;
        //TODO check if push
    }
    fprintf(stderr, "heyo end\n");
    return (true);
}

bool	greedy_sort(t_state *s)
{
    size_t i = get_stack_size(s->curr_stack);

	while (i-- > 0)
		if (do_cheapest_num(s) == false)
			return (false);
	return (true);
}
