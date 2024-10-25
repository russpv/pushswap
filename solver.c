#include "pushswap.h"

// create two more arrays that store '1' where a x-tile boundary is
// calculate the median somewhere
// if above median if !flipped push, else rotate then push + flipped
// you are left with a x-tile at top of stack from a prior sort
// rotate the destination stack to correct ascending order
// repeat on that x-tile and push to opposite stack until no more on current stack
// bubble sort swap each x-tile on stack a to finish NO - rotate+swap, might as well push
// insertion sort

// figure out how to manage how many passes to do
// if n>500, n-2n per divide round k, n/(2^k)-n/(2^k)*(n/2) per insertion round * x-tiles
// so for k=2, 4ths, 1500 - >31000 moves
// for k=3, 8ths, 2000 - >16000 moves
// for k=4, 16ths, 2500 - >8000 moves
// for k=5, 32nds, 3000 - >4000 moves

// Recursive for each array group
// We start with a populated stack
// If we are mid-split-round, need to rotate the bottom half back to bottom
// e.g. 44444 | 11111 | 22222 | 33333 >>> 11111 | ... | 44444
// End up inverted on stack b so insertion sort can skip rotates
// First pass do b: 00000000 >> 2222 | 1111 inverted
// Second pass do a: 33 | 44 || 1111, 22 | 33 | 44 | 11 >> 11 | 22 | 33 | 44; rightside
// Third pass do b: 8 | 7 || 33 | 22 | 11 >> 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1; inverted
// Fourth pass do a:  rightside

//dummy
# define SMALL_NUMS 200
# define LARGE_NUMS 600
# define PASSES 4

// no matter the state, the current partition will be absolutely larger or 
// smaller than whats on the dest stack.
// B>A will be smaller
// A>B will be larger
// I want this to run only B>A, the final pass.
// can write this per partition or per stack. might as well per stack.
/* This is run only on Stack B */
void	insertion_sort(t_state *s)
{
    
	int i = 0;
    const t_stack_ptr b = s->curr_stack;
    const t_stack_ptr a = s->dest_stack;
	const t_partition_ptr partition = create_partition(a);
	while (i < (int)s->nums) /* iterate over all nums */
	{
		size_t rotates = 0;
        size_t rev_rotates = 0;
		int rot_counter = 0;
        int rev_counter = 0;

        if (peek_next_stack(b) > peek_stack(b)) /* swap */
            move(b, SWAP);
		const long num = peek_stack(b);
        if (get_stack_size(a) >= 1)
        {
            fprintf(stderr, "stack size=_%zu\n", get_stack_size(a)); fflush(stderr);
			if (num > peek_bottom(a)) /* new maximum */
				rotates = 1;
			else if (num < peek_stack(a)) /* new min */
			   ;
			else	
			{
				while (num > get_stack_num(a, rotates) && \
                    	rotates < get_stack_size(a))
                	rotates++; 
            	while (num < get_stack_num(a, get_stack_size(a) - 1 - rev_rotates) && \
                	    rev_rotates < get_stack_size(a))
                	rev_rotates++;
			}
        }
        bool do_rotates = false;
		bool do_rev_rotates = false;
        fprintf(stderr, "Counts Rot:%zu Rev:%zu\n", rotates, rev_rotates);
		if (rev_rotates == 0 && rotates == 0) /* new min num */
			;
		else if (rev_rotates > rotates && rotates > 0 && get_stack_size(a) > 1)
        {
            do_rotates = true;
			fprintf(stderr, "Rotating\n");
            while (rotates-- > 0)
            {
                move(a, ROTATE);
                rot_counter++;
            }
        }
        else if (rev_rotates <= rotates && rev_rotates > 0 && get_stack_size(a) > 1) 
        {
            do_rev_rotates = true;
			fprintf(stderr, "Reverse rotating\n");
			rev_counter = 1; /* have to reverse the just-pushed num */
            while (rev_rotates-- > 0)
            {
                move(a, REV_ROTATE);
                rev_counter++;
            }
        }
        move(a, PUSH, pop_stack(b), partition);
		if (rev_rotates == 0 && rotates == 1) /* new max num */ 
        {
            mylog("new maximum\n");
			move(a, ROTATE);
        }
        // check if the next num should be pushed on the way back
        else if (get_stack_size(a) == 2)
		{
			if (rotates > 0)
			{
				mylog("Rotating\n");
				move(a, ROTATE);
			}
		}
        else if (do_rotates == true)
		{
            while (rot_counter-- > 0)
			{
                move(a, REV_ROTATE); 
				if (peek_stack(b) < peek_stack(a) && peek_stack(b) > peek_bottom(a))
					move(a, PUSH, pop_stack(b), partition);
			}
		}
        else if (do_rev_rotates == true)
		{
            while (rev_counter-- > 0)
			{
                move(a, ROTATE);
				if (peek_stack(b) < peek_stack(a) && peek_stack(b) > peek_bottom(a))
					move(a, PUSH, pop_stack(b), partition);
			}
		}
        i++;
	}
	print_stacks(s);
}	

void    swap_both_if_needed(t_state *s)
{
    if (s->curr_stack == s->stacks[STACK_A])
    {
        if ((get_stack_size(s->dest_stack) >= 2) && \
				(get_stack_size(s->curr_stack) >= 2) && \
                (peek_stack(s->curr_stack) > peek_next_stack(s->curr_stack)) && \
                (peek_stack(s->dest_stack) < peek_next_stack(s->dest_stack)))
        {
            mylog("Solve: swapping both\n");
            swap_both(s);
        }
    } else {
        if ((get_stack_size(s->dest_stack) >= 2) && \
				(get_stack_size(s->curr_stack) >=2 ) && \
                (peek_stack(s->curr_stack) < peek_next_stack(s->curr_stack)) && \
                (peek_stack(s->dest_stack) > peek_next_stack(s->dest_stack)))
        {
            mylog("Solve: swapping both\n");
               swap_both(s);
        }
    }
}

void    swap_stack_if_needed(t_state *s)
{
    if (s->dest_stack == s->stacks[STACK_A] && \
        (peek_stack(s->curr_stack) < peek_next_stack(s->curr_stack)))
        move(s->curr_stack, SWAP, 0, NULL, PRINT_ON);
    else if (s->dest_stack == s->stacks[STACK_B] && \
            (peek_stack(s->curr_stack) > peek_next_stack(s->curr_stack)))
        move(s->curr_stack, SWAP, 0, NULL, PRINT_ON);
}

void    process_partition(t_state *s, t_partition_ptr top_partition, t_partition_ptr partition, size_t limit)
{
    size_t i = 0;
    int rot_counter = 0;
    t_partition_ptr dest_partitions[2];
    create_destination_partitions(s, &dest_partitions);
    while (i < limit)
    {
        mylog( "Solve: %zu loop started (%zu moves remaining p:%d, %zu p's)\n", i, \
                get_partition_size(partition),\
                get_partition_id(partition),
                get_partition_count(s->curr_stack));
        print_stacks(s); 
        swap_both_if_needed(s);
        if (peek_stack(s->curr_stack) <= s->pivot)
        {
            if (peek_next_stack(s->curr_stack) <= s->pivot && get_stack_size(s->curr_stack) > 2)
                swap_stack_if_needed(s);
            mylog( "Solve: %ld below median %ld\n", peek_stack(s->curr_stack), s->pivot);
            mylog( "Solve: push lower\n");
            move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[0]);
            print_stacks(s);
            // to STACKB, largers stay on top
            if (s->dest_stack == s->stacks[STACK_B]) {
                if (partition == top_partition) /* if first partition sorting, only rotate */
                {
                    mylog( "Solve: rot\n");
                    move(s->dest_stack, ROTATE); /* TODO: wasted move if all same grouping on stack */
                } /* if not top partition, don't rotate smallers */
            }
            // back to STACKA, smallers go to bottom temporarily
            else if (s->dest_stack == s->stacks[STACK_A] && partition != top_partition) { /* never an empty stack, except first partition sorting */
                mylog( "Solve: rot + banking revrot\n");
                move(s->dest_stack, ROTATE);
                rot_counter++;
            }
        }
        else //value is large
        {
            if (peek_next_stack(s->curr_stack) > s->pivot && get_stack_size(s->curr_stack) > 2)
                swap_stack_if_needed(s);
            mylog( "Solve: push larger\n");
            move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[1]);
            if (s->dest_stack == s->stacks[STACK_B]) // largers stay on top, so temporarily go bottom
            {
                if (partition != top_partition) /* if not top partition, rotate largers and reverse rotate later */
                {
                    mylog("Solve: rot + reverse later\n");
                    move(s->dest_stack, ROTATE);
                    rot_counter++;
                } /* if top partition, don't rotate */
            }
            else if (s->dest_stack == s->stacks[STACK_A] &&  partition == top_partition)
            {
                mylog("Solve: rot\n");
                move(s->dest_stack, ROTATE);
            }
        }
        print_stacks(s);
        mylog( "##(end round)##\n\n"); fflush(stderr);
        i++;
    }
    while (rot_counter-- > 0) { 
        mylog( "Solve: rot counter rot\n");
        move(s->dest_stack,REV_ROTATE); // move bottoms
        print_stacks(s);
    }
}

void	solve(t_state *s)
{
	const t_partition_ptr	top_partition = get_top_partition(s->curr_stack);
    size_t j = 0;
    const size_t partitions = get_partition_count(s->curr_stack);

	++s->curr_pass;
	mylog( "\n########## PASS_%d ###\n", s->curr_pass);
	if (s->curr_pass == PASSES)
    {
        fprintf(stderr, "Doing greedy sort.\n");
		greedy_sort(s);
        return ;
	}
	if (is_sorted_asc(s->stacks[STACK_A]) && is_full(s->stacks[STACK_A])) {
		mylog( "Stack A is sorted!! Returning ...\n");
		return;
	}
	if (s->curr_pass > s->passes) {
        mylog( "Passes finished. Returning...\n");
		return;
    }
	while (j < partitions)
    {
        t_partition_ptr curr_partition = get_top_partition(s->curr_stack);
		mylog("####### PASS %d, LOOP %zu ##\n", s->curr_pass, j);
        mylog( "Solve: top partition: %p size:%zu id:%d value:%ld\n", curr_partition, \
                get_partition_size(curr_partition),\
                    get_partition_id(curr_partition), \
                    peek_partition(curr_partition));
        if (!curr_partition)
            err("Error: partition get error", s);
        s->pivot = get_top_partition_median(s->curr_stack);
        mylog( "Solve: median value %ld\n", s->pivot);
	    process_partition(s, top_partition, curr_partition, get_partition_size(curr_partition));
        j++;
    }
	print_stacks(s);
	flip_curr_stack(s); /* finished with all stack partitions, next split */
	mylog( "Solve: calling solve()\n");
	solve(s);
}

void	solver(t_state *s)
{
	if (s->nums < SMALL_NUMS)
		s->passes = PASSES;
	else if (s->nums < LARGE_NUMS)
		s->passes = PASSES;
	solve(s);
	return ;
}

