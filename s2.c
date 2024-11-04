#include "pushswap.h"


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
    int result;
    if (get_stack_size(s) == 1)
        result = 0;
    else if (idx <= get_middle_idx(s))
        result = idx;
    else 
        result = (idx - get_stack_size(s)); //MUST BE NEGATIVE
    mydebug("-- (get_depth) -- for num:%ld (size:%zu, idx:%d) moves:%d\n",  \
            get_stack_num(s, idx), \
            get_stack_size(s), \
            idx, \
            result);
    return (result);
}

/* Returns +ve or -ve moves to correct insertion position in s
 * Translates insertion position into rotates and reverse rotates 
 * Returns max number if position is beyond middle index 
 */
static void  count_moves_to_sorted_position(t_stack_ptr s, \
		long num, int *rotates, int *reverse_rotates)
{
    int pos = get_insert_position(s, num);
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
    mydebug("---- (count moves) -- pos:%d midx:%d rots:%d rrotes:%d\n", \
            pos, get_middle_idx(s), *rotates, *reverse_rotates);
}

/* Returns -ve or +ve moves to put num in sorted position in s
 * negative for reverse rotations; positive for normal rotations 
 * Case 1: stack too small for moves, returns 0
 * Case 2: moves are beyond midpoint, return opposite moves
 */
static int get_target_depth(t_stack_ptr s, long num)
{
    int rotates;
    int reverse_rotates;
    
    if (get_stack_size(s) < 2)
        return (0);
    count_moves_to_sorted_position(s, num, &rotates, &reverse_rotates);
    
    mydebug("-- (get_target_depth) -- for num %ld (size:%zu): target depth: %d, rev_rots:%d rots:%d\n", \
            num, \
            get_stack_size(s), \
            abs_min(rotates, reverse_rotates), \
            reverse_rotates, rotates);

    if (rotates == INT_MAX)
        return (reverse_rotates);
    return (rotates);
}

/* Returns total moves to rotate and push a number to sorted position; absolute value */
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

/* Saves 'depth' or moves for idx num into s
 */
static int get_total_moves(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int one = get_depth(s->curr_stack, idx);
    const int two = get_target_depth(s->dest_stack, num);

    if (one == INT_MAX || two == INT_MAX || num == LONG_MAX)
        return (INT_MAX);
    s->depth = one;
    s->target_depth = two;
    s->moves = get_moves(one, two);
    mydebug("-- (get_total_moves) -- idx:%d num:%d depth:%d dest_depth:%d (push 1)\n", idx, num, one, two);
    return (s->moves);
}

static long distance(long a, long b)
{
    return (myabs(a - b));
}

static int  increment(int i, int direction)
{
    if (direction < 0)
        return (--i);
    else
        return (++i);
}

/* Returns idx unless conditions met:
 * If a push candidate's destination stack insertion position ("depth") in terms of 
 * moves is absolutely higher than the moves it takes to bring it to the top of current stack
 * for the push, this looks for alternative "closer" candidates using "free" double
 * moves. If t_depth is -ive, we increment negatively
 * using get_stack_idx_at_depth allows index to wrap around. 
 * note: piggy back rotates may wrap around the current stack
 * idx: This is the idx on currstack for the lowest move num
 */ 
static int  find_alts(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int depth = s->depth; //TODO delete
    const int t_depth = s->target_depth; //TODO delete
    const long exposed_num = get_stack_num(s->dest_stack, get_stack_idx_at_depth(s->dest_stack, t_depth));
    long min_distance = distance(exposed_num, num);
    long candidate;
    int i = 0;
    int new_idx = idx;

    mydebug("\033[34m---- (find alts) -- nonzero target depth, testing destnum:%ld t_depth:%d, vs num:%ld idx:%d depth:%d dist:%ld\033[0m\n", \
            exposed_num, t_depth, num, idx, depth, min_distance);
    if (t_depth == 0 || myabs(t_depth) <= idx || exposed_num == LONG_MAX || distance(exposed_num, num) == 1)
    {
        mydebug("\033[31m---- (Find alts) -- aborting, no potential for piggy backs \n\033[0m");
        return (idx);
    }
    // i needs to be an offset from idx, not idx. 
    while (myabs(i) < myabs(t_depth)) /* i++ represents a rotate, i-- a revrot;  */
    {
        i = increment(i, t_depth);
        candidate = get_stack_num(s->curr_stack, get_stack_idx_at_depth(s->curr_stack, i + idx));
        if (candidate == LONG_MAX)
            break ;
        if ((candidate < exposed_num) && distance(candidate, exposed_num) < min_distance)
        {
            mydebug("\033[44m---- (Find alts) -- found closer num:%d\n\033[0m", candidate);
            new_idx = get_stack_idx_at_depth(s->curr_stack, i + idx);
            s->depth = new_idx; 
            min_distance = distance(candidate, exposed_num);
        }
        else ;
          // mydebug("\033[34m---- (Find alts) -- cand:%ld idx:%d dist:%ld failed...\n\033[0m", \
           //        candidate, i + idx, distance(candidate, exposed_num));
        //mydebug("\033[34m---- (Find alts) -- tested cand:%ld idx:%d\n\033[0m", candidate, i + idx);
    }
    mydebug("\033[34m---- (Find alts) -- returning num:%ld idx:%d depth:%d (iter:%d)\n\033[0m", get_stack_num(s->curr_stack, new_idx), \
            new_idx, \
            s->depth, \
            i);
    return (new_idx);
}

/* For tiebreaking: Employs extrema check and biases for numbers closer to deststack number
 * Assumes both have equal moves-to-sorted-position
 * Returns true if curr_idx is closer than ref_idx.
 * Case 1: receives an invalid reference, must return true
 * Case 2: current index moves are less than ref, return true
 * Case 3: current index ties with ref, but is extremum (top or bottom), return false
 * Case 4: current index ties with ref, not extremum, and smaller diff, return true
 */
bool    closer_than(t_state *s, int curr_idx, int ref_idx, int ref_moves)
{
    const int dest_idx = get_stack_idx_at_depth(s->dest_stack, s->target_depth);
    if (dest_idx == INT_MAX)
        return (false);
    if (ref_idx == -1) /* nothing to compare, first try */
        return (true);
    const long dest_num = get_stack_num(s->dest_stack, dest_idx);
    const long curr_num = get_stack_num(s->curr_stack, curr_idx);
    const long ref_num = get_stack_num(s->curr_stack, ref_idx);

    if (dest_num == LONG_MAX || curr_num == LONG_MAX || ref_num == LONG_MAX)
        return (false);
    if (get_moves(s->depth, s->target_depth) < ref_moves)
        return (true);
    else if (get_moves(s->depth, s->target_depth) > ref_moves)
        return (false);
    mydebug("\033[32m---- (closer_than) >> checking extremum (%ld destnum %d moves, %d depth, %d t_depth)...\n\033[0m", \
            dest_num, ref_moves, s->depth, s->target_depth);
    if (is_stack_extremum(s->dest_stack, curr_num) ^ is_stack_extremum(s->dest_stack, ref_num))
    {
        if (is_stack_extremum(s->dest_stack, curr_num) \
        && !(is_stack_extremum(s->dest_stack, ref_num))) {
            mydebug("\033[32m---- (closer_than) >> extremum, not closer.\n\033[0m");
            return (false);}
    }
    if (distance(curr_num, dest_num) < distance(ref_num, dest_num))
    {
        mydebug("\033[32m---- (closer_than) >> closer number! (%d - %d) v (%d - %d)\n\033[0m", \
                curr_num, dest_num,
                ref_num, dest_num);
        return (true);
    }
    return (false);
}

/* Returns iterator that is skipped ahead if a 'shallow' move already found AND
 * a number of indices equal to the best move have already been tested from the
 * top of the stack.
 * Move is shallow if total moves is less than or equal to 2 or 10% of N whichever greater 
 * (No sense testing on current stack size of less than 20% of N or 2 * interval)
 * The skip amount is the best move count away from the bottom of the stack 
 */
int skip_pointless_indices(t_state *s, int min_idx_moves, const int i)
{
    const size_t curr_stack_size = get_stack_size(s->curr_stack);
    const size_t dest_stack_size = get_stack_size(s->dest_stack);
    int new_i = i;

    if (3 * curr_stack_size < dest_stack_size)
        return (new_i);
    if (min_idx_moves <= abs_max(s->nums / 10, 2) && i >= min_idx_moves)
    {
       new_i = abs_max(curr_stack_size - min_idx_moves, i);
        if (new_i != i)
            mydebug("\033[47m-- (skip_indices) -- skip to idx:%d (csz:%zu dsz:%zu, min_moves:%d) \n\033[0m", \
                new_i, curr_stack_size, dest_stack_size, min_idx_moves);
    }
    return (new_i); 
}

/* Returns index of current stack number that gives the lowest moves
 * Unless a closer number exists for that number's sorted position in
 * destination stack for the same amount of moves.
 * Skips over numbers in middle of stack where sensible 
 */
static int find_cheapest_idx(t_state *s)
{
    int i;
    i = 0;
    
    s->tmp = -1;
    while (i < (int)get_stack_size(s->curr_stack))
    {
        mydebug("\033[33m-- (find_cheapest) -- testing: num:%d idx:%d moves:%d\n\033[0m", \
                get_stack_num(s->curr_stack, i), i, get_total_moves(s, i));
        if (get_total_moves(s, i) == 1) 
            return (i);
        if (closer_than(s, i, s->tmp, s->tmp_moves) == true)
        {
            mydebug("\033[33m-- (find_cheapest) -- result: new min (%ld  i:%d) for %d moves\n\033[0m", \
                    get_stack_num(s->curr_stack, i), i, get_total_moves(s, i));
            s->tmp = i;
            s->tmp_moves = s->moves;
            s->tmp_depth = s->depth;
            s->tmp_t_depth = s->target_depth ;
        }
        i = skip_pointless_indices(s, s->tmp_moves, i) + 1;
    }
    s->depth = s->tmp_depth;
    s->target_depth = s->tmp_t_depth;
    if (s->target_depth > 0  && get_stack_size(s->curr_stack) > 1)
        s->tmp = find_alts(s, s->tmp);
    mydebug("\033[43m-- (find cheapest) -- Searched %d idx's, Found cheapest i:%d n:%ld moves:%d d:%d td:%d \033[0m\n", \
            i, s->tmp, get_stack_num(s->curr_stack, s->tmp), s->tmp_moves, s->depth, s->target_depth);
    return (s->tmp);
}


// If sign of depth opposite sign of target depth, then rotate both
/* Interprets negative moves as reverse rotates */
/* Does not rotate back to starting position after push */
static bool    do_cheapest_num(t_state *s)
{
    mydebug("###heyo for size %zu num %ld\n", get_stack_size(s->curr_stack), peek_stack(s->curr_stack));
    print_stacks(s); mydebug("\n");
    if (!is_sorted_asc_rotationally(s->dest_stack))
        err("WTFFFF!!!\n", s);
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
