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
# define SORT_STOP 20

// no matter the state, the current partition will be absolutely larger or 
// smaller than whats on the dest stack.
// B>A will be smaller
// A>B will be larger
// I want this to run only B>A, the final pass.
// can write this per partition or per stack. might as well per stack.
void	insertion_sort(t_state *s)
{
	int i = 0;
	const t_partition_ptr partition = create_partition(s->dest_stack);
	while (i < (int)s->nums)
	{
		const long num = peek_stack(s->curr_stack);
		const t_stack_ptr b = s->curr_stack;
		const t_stack_ptr a = s->dest_stack;
		size_t rotates = 0;
		int rot_counter = 0;
		while (num > get_stack_num(a, rotates) && \
				rotates < get_stack_size(a))
			rotates++;
		while (rotates-- > 0)
		{
			move(a, ROTATE);
			rot_counter++;
		}
        move(a, PUSH, pop_stack(b), partition);
		while (rot_counter-- > 0)
            move(a, REV_ROTATE);
        i++;
	}
	print_stacks(s);
}	

void	solve(t_state *s)
{
	t_partition_ptr	top_partition = NULL;
	size_t i;
    size_t j = 0;
	size_t limit;
    int rot_counter = 0;
    const size_t partitions = get_partition_count(s->curr_stack);
	t_partition_ptr dest_partitions[2];

	++s->curr_pass;
	mylog( "\n########## PASS_%d ###\n", s->curr_pass);
	if (s->dest_stack == s->stacks[STACK_A] && get_partition_size(get_top_partition(s->curr_stack)) < SORT_STOP) {
		fprintf(stderr, "Too few to quicksort, insertion sort.\n");
		insertion_sort(s);
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
        i = 0;
        top_partition = get_top_partition(s->curr_stack);
		mylog("####### PASS %d, LOOP %zu ##\n", s->curr_pass, j);
        mylog( "Solve: top partition: %p size:%zu id:%d value:%ld\n", top_partition, \
                get_partition_size(top_partition),\
                    get_partition_id(top_partition), \
                    peek_partition(top_partition));
        if (!top_partition)
            err("Error: partition get error", s);
        s->pivot = get_top_partition_median(s->curr_stack);
        mylog( "Solve: median value %ld\n", s->pivot);
        create_destination_partitions(s, &dest_partitions);
        mylog( "Solve: two partitions created\n");
	    limit = get_partition_size(top_partition);
        rot_counter = 0;
        while (i < limit)
	    {
            mylog( "Solve: %zu loop started (%zu moves remaining p:%d, %zu p's)\n", i, \
                    get_partition_size(top_partition),\
                    get_partition_id(top_partition),
					partitions);
            print_stacks(s); 
            if (peek_stack(s->curr_stack) <= s->pivot)
            {
                mylog( "Solve: %ld below median %ld\n", peek_stack(s->curr_stack), s->pivot);
                mylog( "Solve: push\n");
                move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[0]);
                print_stacks(s);
				// to STACKB, largers stay on top
            	if (s->dest_stack == s->stacks[STACK_B]) {
					if (get_partition_count(s->dest_stack) <= (size_t)2)
					{
						mylog( "Solve: rot\n");
                   		move(s->dest_stack, ROTATE); /* TODO: wasted move if all same grouping on stack */
					}
                }
				// back to STACKA, smallers go to bottom temporarily
				else if (s->dest_stack == s->stacks[STACK_A]) {
					mylog( "Solve: rot + banking revrot\n");
					move(s->dest_stack, ROTATE);
                    rot_counter++;
            	}
			}
            else //value is large
            {
                mylog( "Solve: push\n");
                move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[1]);
                if (s->dest_stack == s->stacks[STACK_B]) // largers stay on top, so temporarily go bottom
                {
					if (get_partition_count(s->dest_stack) > (size_t)2)
                    {
						mylog("Solve: rot\n");
						move(s->dest_stack, ROTATE);
                    	rot_counter++;
					}
                }
            }
            print_stacks(s);
            mylog( "##(end round)##\n\n"); fflush(stderr);
            i++;
        }
        while (--rot_counter >= 0) { 
            mylog( "Solve: rot counter rot\n");
            move(s->dest_stack,REV_ROTATE); // move bottoms
			print_stacks(s);
        }
        dest_partitions[0] = NULL;
        dest_partitions[1] = NULL;
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
		s->passes = 10;
	else if (s->nums < LARGE_NUMS)
		s->passes = 10;
	solve(s);
	return ;
}

