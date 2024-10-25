#include "pushswap.h"

// Some version of the greedy search
// The only requirement is that the destination be in stack order

// TODO
int abs_min(int a, int b)
{
    if (a < b)
        return (a);
    else if (b < a)
        return (b);
    else
        return (a);
}

/* Positive for rotations; negative for reverse rots */
int get_depth(t_stack_ptr s, int idx)
{
    if (idx < get_middle_idx(s))
        return (idx);
    else 
        return (idx - (get_stack_size(s) - 1));
}

size_t  count_moves_to_sorted_position(t_stack_ptr s, long num, (*move))
{
    int i;
    size_t counter;
    t_stack_ptr c;

    i = 0;
    counter = 0;
    c = copy_stack(s);
    push_stack(c, num, create_partition());
    while (i < get_middle_idx(c))
    {
        move(); //TODO
        counter++;
        if (is_sorted_asc(c))
            break ;
    }
    destroy_stack(c);
    return (counter);
}

/* Negative for rotations; positive for reverse rots */
int get_target_depth(t_stack_ptr s, long num)
{
    size_t rotates;
    size_t reverse_rotates;
    
    rotates = 0;
    reverse_rotates = 0;
    rotates = count_moves_to_sorted_position(rotates, num, ROTATE);
    reverse_rotates = count_moves_to_sorted_position(reverse_rotates, num, REVERSE);

    return (abs_min(rotates, reverse_rotates)); //TODO
}

/* Returns moves to rotate and push a number to sorted position; absolute value */
int get_total_moves(t_state *s, int idx)
{
    const long num = get_num(s->curr_stack, idx);
    const int one = get_depth(s->curr_stack, idx);
    const int two = get_target_depth(s->dest_stack, num);

    if (one >= 0 && two >= 0)
        return (one + two + 1);
    else if (one < 0 && two < 0)
        return (-one + -two + 1);
    else if (one < 0 && two >= 0)
        return (one + two + 1 + min(-one, two));
    else if (one >= 0 && two < 0)
        return (one + two + 1 + min(one, -two));
    else
        return (INT_MAX);
}

/* Loop each num, return minimum score idx */
int find_cheapest_idx(t_state *s)
{
    int minimum;
    int i;

    i = 0;
    minimum = INT_MAX;
    while (i < get_stack_size(s->curr_stack))
    {
        a = get_total_moves(s->curr_stack, i);
        if (a < minimum)
            minimum = i;
        i++;
    }
    return (minimum);
}

int find_dest_idx

/*******/

// If sign of depth opposite sign of target depth, then rotate both

bool    do_cheapest_num(t_state *s)
{
    const int   src_idx = find_cheapest_idx(s);
    const int   dest_idx = find_dest_idx(s, src_idx);
    dest_moves = get();
    src_moves = get();
    // if same sign moves, do dual moves;
    while (dest_moves < 0 && src_moves < 0)
    {
        move(REVERSE_BOTH);
        dest_moves++;
        src_moves++;
    }
    while (dest_moves > 0 && src_moves > 0)
    {
        move(ROTATE_BOTH);
        dest_moves--;
        src_moves--;
    }
    while (src_moves < 0)
    {
        move(REVERSE);
        dest_moves++;
    }
    while (src_moves > 0)
    {
        moves(ROTATE);
        src_moves--;
    }
    move(PUSH);
    while (dest_moves < 0)
    {
        move(REVERSE);
        dest_moves++;
        // TODO check if push, or would this never happen
    }
    while (dest_moves > 0)
    {
        move(ROTATE);
        dest_moves--;
        //TODO check if push
    }
    return (true);
}
