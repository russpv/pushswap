#include "pushswap.h"

// Some version of the greedy search
// The only requirement is that the destination be in stack order
//

static long myabs(long a)
{
    if (a < 0)
        return (-a);
    else
        return (a);
}

/* Returns absolute value minimum and original args */
static int abs_min(int a, int b)
{
	const long i = myabs((long)a);
	const long j = myabs((long)b);

    if (i < j) {
//        mydebug("ABS: %d less than %d\n", a, b);
        return (a);
    } else if (j < i) {
//        mydebug("ABS: %d greater than %d\n", a, b);
        return (b);
    } else {
//        mydebug("ABS: %d equal to %d\n", a, b);
        return (a);
    }
}

static int  abs_max(int a, int b)
{
    const long i = myabs((long)a);
    const long j = myabs((long)b);

    if (i < j)
        return (b);
    else if (i > j)
        return (a);
    else
        return (a);
}

/* Positive for rotations; negative for reverse rots */
static int get_depth(t_stack_ptr s, int idx)
{
    if (get_stack_size(s) == 1) {
 //       mydebug("get_depth -- last member 0\n");
        return (0);
    }
    else if (idx <= get_middle_idx(s))
    {
 //       mydebug("get_depth (num:%ld in size:%zu) -- Idx:%d (depth) less that middle idx:%d\n", peek_stack(s), get_stack_size(s), idx, get_middle_idx(s));
        return (idx);
    }
    else 
    {
 //       mydebug("get_depth (%ld in size:%zu) -- idx %d greater than middle idx %d, returning :%d\n", peek_stack(s), get_stack_size(s), idx, get_middle_idx(s), \
 //               idx- (get_stack_size(s)));
        return (idx - (get_stack_size(s)));
    }
}

/* Expects s to be the destination stack */
static size_t  count_moves_to_sorted_position(t_stack_ptr s, \
		long num, enum e_move_type m)
{
    int i;
    size_t counter;
    t_stack_ptr c; /* dest stack */

    i = 0;
    counter = 0;
    c = copy_stack(s);
    if (!c)
		return (SIZE_MAX);
    if (get_stack_size(c) == 0 || get_stack_size(c) == 1)
        return (0);
    while (i <= get_middle_idx(c))
    {
      //  mydebug("\nget_target_depth -- test w/ push (%ld) :", num);
        if (push_stack(c, (int)num, create_partition(c)) == false)
            mydebug(RED"get_target_depth -- push error num_%d \n"RESET, (int)num);
        print_stack(c);
        if (is_sorted_asc_rotationally(c) == true)
            break ;
      //  mydebug("get_target_depth -- not sorted (counter_%zu), popping and rot'g...\n", counter);
      //  mydebug("get_target_depth -- popped %ld\n", pop_stack(c)); //take off the number and rotate
        move(c, m, 0, NULL, PRINT_OFF);
        print_stack(c);
        counter++;
		i++;
    }
    if (i > get_middle_idx(c))
        counter = SIZE_MAX;
    //mydebug("get_target_depth -- returning with counter_%zu\n", counter);
    destroy_stack(c);
    return (counter);
}

/* For num in stack s, returns number of moves if pushed to it
 * negative for reverse rotations; positive for normal rotations */
static int get_target_depth(t_stack_ptr s, long num)
{
    size_t rotates;
    size_t reverse_rotates;
    
    if (get_stack_size(s) < 2)
        return (0);
    rotates = count_moves_to_sorted_position(s, num, ROTATE);
    if (rotates != SIZE_MAX)
        reverse_rotates = get_stack_size(s) - rotates;
    else
        reverse_rotates = count_moves_to_sorted_position(s, num, REV_ROTATE);
    mydebug("get target depth -- reverse_rotates: %zu, rots:%zu size:%zu\n", reverse_rotates, rotates, get_stack_size(s));
    if (rotates == SIZE_MAX)
        rotates = INT_MAX;;
    mydebug("get_target_depth -- Got %ld's target depth: %d, rots:%d rrs:%zu\n", num, \
            abs_min(rotates, reverse_rotates), \
            rotates, (int)reverse_rotates);
    if (reverse_rotates < rotates)
        return (-(int)reverse_rotates);
    return (rotates);
}

/* Returns moves to rotate and push a number to sorted position; absolute value */
/* If either both reverse and normal rotations involved, takes the absolute value maximum */
/* (since double rots are possible */

static int  get_moves(const int one, const int two)
{
    if (one >= 0 && two >= 0) // only rotations
        return (myabs(abs_max(one, two)) + 1);
    else if (one < 0 && two < 0) // only reverse rotations
        return (myabs(abs_max(one, two)) + 1);
    
    
    else if (one < 0 && two >= 0) // mixture, no discount 
        return (myabs(one) + two + 1 );
    else if (one >= 0 && two < 0) // mixture, no discount
        return (one + myabs(two) + 1 );
    else
        return (INT_MAX);
}

static int get_total_moves(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int one = get_depth(s->curr_stack, idx);
    const int two = get_target_depth(s->dest_stack, num);

    if (one == INT_MAX || two == INT_MAX || num == LONG_MAX)
        return (INT_MAX);
    s->depth = one;
    s->target_depth = two;
    //mydebug("get_total_moves -- Idx:%d num:%d depth:%d dest_depth:%d (push 1)\n", idx, num, one, two);
    return (get_moves(one, two));
}

static long distance(long a, long b)
{
    return (myabs(a - b));
}


/* If target stack depth is higher than depth to push candidate on curr stack
 * Test if piggy back rotates expose closer numbers
 */ 
static int  find_alts(t_state *s, int idx)
{
    mydebug("\033[34mFind alts -- calling variables...\n");
    const long num = get_stack_num(s->curr_stack, idx);
    mydebug("\033[34mFind alts -- ref num %d\n", num);
    const int depth = s->target_depth;
    mydebug("\033[34mFind alts -- target depth %d\n", depth);
    const long exposed_num = get_stack_num(s->dest_stack, get_stack_idx_at_depth(s->dest_stack, depth + 1));
    mydebug("\033[34mFind alts -- dest num &d\n", exposed_num);
    long min_distance = distance(exposed_num, num);
    long candidate;
    int i = idx;
    mydebug("\033[34mFind alts -- nonzero depth, testing destnum:%ld depth:%d, vs num:%ld idx:%d dist:%ld\033[0m\n", \
            exposed_num, depth, num, idx, min_distance);
    if (depth == 0 || myabs(depth) <= idx || exposed_num == LONG_MAX)
    {
        mydebug("\033[34mFind alts -- problem with search, aborting \n\033[0m");
        return (idx);
    }
    while (i++ < myabs(depth)) /* i++ represents a rotate */
    {
        candidate = get_stack_num(s->curr_stack, i);  //TODO aint gonna work, need a circular nextnum()
        if (candidate == LONG_MAX)
            break ;
        mydebug("\033[34mFind alts -- cand:%ld idx:%d dist:%ld testing...\n\033[0m", candidate, idx, distance(candidate, exposed_num));
        if (distance(candidate, exposed_num) < min_distance)
        {
            mydebug("\033[34mFind alts -- found closer num:%d\n\033[0m", candidate);
            idx = i;
            min_distance = distance(candidate, exposed_num);
        }
        else
           mydebug("\033[34mFind alts -- cand:%ld idx: dist:%ld failed...\n\033[0m", candidate, idx, distance(candidate, exposed_num));
        mydebug("\033[34mFind alts -- tested cand:%ld idx:%d\n\033[0m", candidate, i);
    }
    mydebug("\033[34mFind alts -- returning num:%d idx:%d\n\033[0m", get_stack_num(s->curr_stack, idx), idx);
    return (idx);
}


/* TODO: Used to compare alternates on currstack for ANY deststack depth  nahh, I need to import moves
 * For tiebreaking: Employs extrema check and biases for numbers closer to deststack number
 * Assumes both have equal moves-to-sorted-position
 * Returns true if curr_idx is closer than ref_idx.
 */
bool    closer_than(t_state *s, int curr_idx, int ref_idx, int ref_moves)
{
    const int dest_idx = get_stack_idx_at_depth(s->dest_stack, s->target_depth);
    mydebug("hey");
    if (dest_idx == INT_MAX)
        return (false);
    if (ref_idx == -1) /* nothing to compare, first try */
        return (true);
    const long dest_num = get_stack_num(s->dest_stack, dest_idx);
    mydebug("hey");
    const long curr_num = get_stack_num(s->curr_stack, curr_idx);
    mydebug("hey - ref_idx %d", ref_idx);
    const long ref_num = get_stack_num(s->curr_stack, ref_idx);
    mydebug("hey");

    if (dest_num == LONG_MAX || curr_num == LONG_MAX || ref_num == LONG_MAX)
        return (false);
//    mydebug("> closer_than -- comparing moves...\n");
    if (get_moves(s->depth, s->target_depth) < ref_moves)
        return (true);
    else if (get_moves(s->depth, s->target_depth) > ref_moves)
        return (false);
    mydebug("> closer_than -- checking extremum...\n");
    if (is_stack_extremum(s->dest_stack, curr_num) ^ is_stack_extremum(s->dest_stack, ref_num))
    {
        if (is_stack_extremum(s->dest_stack, curr_num) \
        && !(is_stack_extremum(s->dest_stack, ref_num)))
            return (false);
        else
            return (true);
    }
    if (curr_num - dest_num < ref_num - dest_num)
        return (true);
    return (false);
}

/* Return true if m->i reduces more entropy than m->min_idx on dest_stack */
/*bool    update_cheapest(struct s_move *m, t_state *s)
{
    if (i_moves < min_idx_moves)
        return (true);
    else if (i_moves > min_idx_moves)
        return (false);
    else if (closer_than(m, s) == true)
        return (true);
    else
        return (false);
}*/

/* Loop EACH num in current stack, return minimum score idx */
/* TODO explore if searching only top and bottom quartiles speeds up */
static int find_cheapest_idx(t_state *s)
{
    int i = 0;
    int i_moves = 0;
    int min_idx = -1;
    int min_idx_moves = INT_MAX;
    const size_t stack_size = get_stack_size(s->curr_stack);
    
    while (i < (int)stack_size)
    {
        //mydebug("Find cheapest -- Testing idx:%d\n", i );
        i_moves = get_total_moves(s, i); /* loads info in state struct */
        //mydebug("\033[33mFind cheapest -- idx_%d total moves %d, testing...\n\033[0m", i, i_moves);
        if (i_moves == INT_MAX){
        //    mydebug("Find cheapest -- get_total_moves error\n");
            return (INT_MAX);
        }
        if (i_moves == 1) { //TODO combine this with below if test
        //    mydebug("Find cheapest -- found total moves 1, pushing\n");
            min_idx = i; // TODO just return here
            min_idx_moves = i_moves;
            break ;
        }
        if (closer_than(s, i, min_idx, min_idx_moves) == true)
        {
        //    mydebug("Find cheapest -- new min is idx:%d for %d moves\n", i, i_moves);
            min_idx = i;
            min_idx_moves = i_moves;
        }
        //mydebug("----next idx----\n");
        if (i == get_stack_size(s->dest_stack) / 2) /* skip pointless indices */
            if (stack_size > 2 * get_stack_size(s->dest_stack))
                i = stack_size - get_stack_size(s->dest_stack) / 2;
        i++;
    }
    //mydebug("----done----\n");
    if (min_idx > 0)
        mylog("\033[31mA MIN IDX THAT ISN'T THE TOP OF STACK!!!!!!!(%d)\033[0m", min_idx);
    if (min_idx_moves != 1 && s->target_depth != 0 && get_stack_size(s->curr_stack) != 1)
        min_idx = find_alts(s, min_idx);
    mydebug("\033[43mFind cheapest -- Searched %d idx's, Found cheapest idx:%d moves:%d\n\033[m", i, min_idx, min_idx_moves);
    return (min_idx);
}


// If sign of depth opposite sign of target depth, then rotate both
/* Interprets negative moves as reverse rotates */
/* Does not rotate back to starting position after push */
static bool    do_cheapest_num(t_state *s)
{
    fprintf(stderr, "###heyo for size %zu num %ld\n", get_stack_size(s->curr_stack), peek_stack(s->curr_stack));
    if(find_cheapest_idx(s) == INT_MAX)
        return (false);
    int	dest_moves = s->target_depth; 
    int	src_moves = s->depth; 
    t_partition_ptr partition;

    if (get_partition_count(s->dest_stack) == 0)
        partition = create_partition(s->dest_stack);
    else
        partition = get_top_partition(s->dest_stack);
    fprintf(stderr, "dest_moves_%d src_moves_%d \n", dest_moves, src_moves);
    while (dest_moves < 0 && src_moves < 0)
    {
        mylog("S2: REVERSING BOTH!\n");
		rev_rotate_both(s);
        dest_moves++;
        src_moves++;
    }
    while (dest_moves > 0 && src_moves > 0)
    {
        mylog("S2: ROTATING BOTH!\n");
        rotate_both(s);
        dest_moves--;
        src_moves--;
    }
    fprintf(stderr, "dest_moves_%d src_moves_%d boths done\n", dest_moves, src_moves);
    while (src_moves < 0)
    {
        fprintf(stderr, "doing rev rotate\n");
        move(s->curr_stack, REV_ROTATE, 0, NULL, PRINT_ON);
        src_moves++;
    }
    fprintf(stderr, "dest_moves_%d src_moves_%d rev_rotates done\n", dest_moves, src_moves);
    while (src_moves > 0)
    {
        move(s->curr_stack, ROTATE, 0, NULL, PRINT_ON);
        src_moves--;
    }
    fprintf(stderr, "dest_moves_%d src_moves_%d rotates done\n", dest_moves, src_moves);
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
    move(s->dest_stack, PUSH, pop_stack(s->curr_stack), partition, PRINT_ON);
    //print_stack(s->dest_stack);
    fprintf(stderr, "heyo end###\n");
    return (true);
}

static bool rotate_back_to_head(t_stack_ptr s)
{
    int   moves;
 
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


bool    greedy_sort(t_state *s)
{
    size_t i = get_stack_size(s->curr_stack);
    mylog("\033[45mGREEDY SORT GREEDY SORT GREEDY SORT GREEDY SORT *********************\n\033[0m");
    mylog("\033[45mGREEDY SORT GREEDY SORT GREEDY SORT GREEDY SORT *********************\n\033[0m");
    mylog("\033[45mGREEDY SORT GREEDY SORT GREEDY SORT GREEDY SORT *********************\n\033[0m");
    mylog("\033[45mGREEDY SORT GREEDY SORT GREEDY SORT GREEDY SORT *********************\n\033[0m");
	while (i-- > 0)
		if (do_cheapest_num(s) == false)
			return (false);
    if (!rotate_back_to_head(s->dest_stack))
        mydebug("rotate back problem\n");
    print_stacks(s);
    mydebug("greedy sort is done.\n");
	return (true);
}
