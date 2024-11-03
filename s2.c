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

/* Expects s to be the destination stack */
/* Loops moves up to and INCLUDING middle index to find correct position 
 * Returns max number if position is beyond middle index 
 * */
static size_t  count_moves_to_sorted_position(t_stack_ptr s, \
		long num, enum e_move_type m)
{
    /*int i;
    size_t counter;
    t_stack_ptr c;

    i = 0;
    counter = 0;
    c = copy_stack(s);
    if (!s)
		return (SIZE_MAX);
    if (get_stack_size(c) == 0 || get_stack_size(c) == 1)
        return (0);
    while (i <= get_middle_idx(c))
    {
      //  mydebug("\nget_target_depth -- test w/ push (%ld) :", num);
        if (push_stack(c, (int)num, get_top_partition(c)) == false)
            mydebug(RED"ERR count_moves  -- push error num_%d \n"RESET, (int)num);
        if (is_sorted_asc_rotationally(c) == true)
            break ;
      //  mydebug("get_target_depth -- not sorted (counter_%zu), popping and rot'g...\n", counter);
      //  mydebug("get_target_depth -- popped %ld\n", pop_stack(c)); //take off the number and rotate
        pop_stack(c);
        move(c, m, 0, NULL, PRINT_OFF);
        counter++;
		i++;
    }
    if (pos > get_middle_idx(c))
    {
        mydebug("-- count_moves -- too many moves, go with reverse\n");
        counter = SIZE_MAX;
    }
    destroy_stack(c); */
    //mydebug("get_target_depth -- returning with counter_%zu\n", counter);
    size_t  moves;
    int pos = get_insert_position(s, num);
    moves = pos;
    if (pos > get_middle_idx(s) && m == ROTATE)
    {
        mydebug("---- (binary search) -- pos_%d > midx:%d\n", pos, get_middle_idx(s));
        moves = SIZE_MAX;
    }
    else if (pos > get_middle_idx(s) && m == REV_ROTATE) 
        moves = get_stack_size(s) - (size_t)pos;
    mydebug("---- (binary search) -- depth:%zu mididx:%d\n", moves, get_middle_idx(s));
    return (moves);
}

/* For num in stack s, returns number of moves if pushed to it
 * negative for reverse rotations; positive for normal rotations 
 * Case 1: stack too small for moves, returns 0
 * Case 2: moves are beyond midpoint, return opposite moves
 * */
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
    if (rotates == SIZE_MAX)
        rotates = INT_MAX;;
    mydebug("-- (get_target_depth) -- for num %ld (size:%zu): target depth: %d, rev_rots:%d rots:%zu\n", num, \
            get_stack_size(s), \
            abs_min(rotates, reverse_rotates), \
            (int)reverse_rotates, rotates);
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
    mydebug("-- (get_total_moves) -- idx:%d num:%d depth:%d dest_depth:%d (push 1)\n", idx, num, one, two);
    return (get_moves(one, two));
}

static long distance(long a, long b)
{
    return (myabs(a - b));
}

/* If target stack depth is higher than depth to push candidate on curr stack
 * Test if piggy back rotates on current stack expose closer numbers
 * note: piggy back rotates may wrap around the current stack
 * idx: This is the idx on currstack for the lowest move num 
 */ 
static int  find_alts(t_state *s, int idx)
{
    const long num = get_stack_num(s->curr_stack, idx);
    const int depth = s->depth;
    const int t_depth = s->target_depth;
    const long exposed_num = get_stack_num(s->dest_stack, get_stack_idx_at_depth(s->dest_stack, t_depth));
    long min_distance = distance(exposed_num, num);
    long candidate;
    int i = idx;
    mydebug("\033[34m---- (find alts) -- nonzero target depth, testing destnum:%ld t_depth:%d, vs num:%ld idx:%d depth:%d dist:%ld\033[0m\n", \
            exposed_num, t_depth, num, idx, depth, min_distance);
    if (t_depth == 0 || myabs(t_depth) <= idx || exposed_num == LONG_MAX)
    {
        mydebug("\033[31m---- (Find alts) -- aborting, no potential for piggy backs \n\033[0m");
        return (idx);
    }
    while (i++ < myabs(t_depth)) /* i++ represents a rotate */
    {
        candidate = get_stack_num(s->curr_stack, get_stack_idx_at_depth(s->curr_stack, i));
        if (candidate == LONG_MAX)
            break ;
        if (distance(candidate, exposed_num) < min_distance)
        {
            mydebug("\033[34m---- (Find alts) -- found closer num:%d\n\033[0m", candidate);
            idx = i;
            min_distance = distance(candidate, exposed_num);
        }
        else
           mydebug("\033[34m---- (Find alts) -- cand:%ld idx:%d dist:%ld failed...\n\033[0m", \
                   candidate, idx, distance(candidate, exposed_num));
        mydebug("\033[34m---- (Find alts) -- tested cand:%ld idx:%d\n\033[0m", candidate, i);
    }
    mydebug("\033[34m---- (Find alts) -- returning num:%d idx:%d depth:%d\n\033[0m", get_stack_num(s->curr_stack, idx), idx, \
            i);
    return (idx);
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
    mydebug("\033[32m---- (closer_than) >> checking extremum (%d moves, %d depth, %d t_depth)...\n\033[0m", \
            ref_moves, s->depth, s->target_depth);
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

/* Returns iterator that is skipped ahead if a 'shallow' move already found 
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
    if (min_idx_moves <= abs_max(s->nums / 10, 2))
    {
       new_i = abs_max(curr_stack_size - min_idx_moves, i);
        if (new_i != i)
            mydebug("\033[47m-- (skip_indices) -- skip to idx:%d (csz:%zu dsz:%zu, min_moves:%d) \n\033[0m", \
                new_i, curr_stack_size, dest_stack_size, min_idx_moves);
    }
    return (new_i);
}

/* Loop EACH num in current stack, return minimum score idx */
/* TODO explore if searching only top and bottom quartiles speeds up */
static int find_cheapest_idx(t_state *s)
{
    int i = 0;
    int i_moves = 0;
    int min_idx = -1;
    int min_idx_moves = INT_MAX;
    const size_t stack_size = get_stack_size(s->curr_stack);
    int depth;
    int t_depth;

    while (i < (int)stack_size)
    {
        //mydebug("Find cheapest -- Testing idx:%d\n", i );
        i_moves = get_total_moves(s, i); /* loads info in state struct */
        mydebug("\033[33m-- (find_cheapest) -- testing num:%d idx_%d total moves %d ...\n\033[0m", \
                get_stack_num(s->curr_stack, i), i, i_moves);
        if (i_moves == INT_MAX){
            mydebug("\033[31mERR Find cheapest -- get_total_moves error\033[0m\n");
            return (INT_MAX);
        }
        if (i_moves == 1) { 
            min_idx = i; 
            min_idx_moves = i_moves;
            depth = s->depth;
            t_depth = s->target_depth ;
            break ;
        }
        if (closer_than(s, i, min_idx, min_idx_moves) == true)
        {
            mydebug("\033[33m-- (find_cheapest) -- result: new min (%ld  i:%d) for %d moves\n\033[0m", \
                    get_stack_num(s->curr_stack, i), i, i_moves);
            min_idx = i;
            min_idx_moves = i_moves;
            depth = s->depth;
            t_depth = s->target_depth ;
        }
        i = skip_pointless_indices(s, min_idx_moves, i);
        i++;

    }
    s->depth = depth; // SAVE THIS FOR EXECUTION
    s->target_depth = t_depth;
    if (min_idx > 0)
        mylog("\033[33mA MIN IDX THAT ISN'T THE TOP OF STACK!!!!!!!(%d)\033[0m\n", min_idx);
    if (min_idx_moves != 1 && s->target_depth != 0  && get_stack_size(s->curr_stack) != 1)
        min_idx = find_alts(s, min_idx);
    mydebug("\033[43m-- (find cheapest) -- Searched %d idx's, Found cheapest idx:%d moves:%d\n\033[m", i, min_idx, min_idx_moves);
    return (min_idx);
}


// If sign of depth opposite sign of target depth, then rotate both
/* Interprets negative moves as reverse rotates */
/* Does not rotate back to starting position after push */
static bool    do_cheapest_num(t_state *s)
{
    fprintf(stderr, "###heyo for size %zu num %ld\n", get_stack_size(s->curr_stack), peek_stack(s->curr_stack));
    print_stacks(s); mydebug("\n");
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
