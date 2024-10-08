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


# define TOP 0
# define SMALL_NUMS 200
# define LARGE_NUMS 600


// at this point, two stacks have been created ...
// conditionally create a starting partition ...
void	solve(t_state *s)
{
	int rot_counter = 0;
	size_t i = 0;
	t_partition_ptr	top_partition;
	t_partition_ptr dest_partitions[2];

	++s->curr_pass;
	if (s->curr_pass == s->passes)
		return; 
	fprintf(stderr, "currsplit:%d##", s->curr_pass); fflush(stderr);
	// TODO another while loop to complete all x-tiles in the pass
	// 1. Split partition on stack
	// 1a. Get or create partition on stack (assume filled partition earlier)
	// 1b. Create two partitions on the destination stack
	// 1c. Iterate and push to destination stack
	top_partition = get_top_partition(s->curr_stack);
	fprintf(stderr, "top partition getted: %p \n", top_partition);
	if (!top_partition)
		err("partition get error", s);
	s->pivot = get_top_partition_median(s->curr_stack);
	create_destination_partitions(s, &dest_partitions);
	fprintf(stderr, "partitions created\n");
	while (i < get_partition_size(top_partition))
	{
		print_stacks(s); 
		if (peek_stack(s->curr_stack) <= s->pivot)
		{
			fprintf(stderr, "%ld below %ld|", peek_stack(s->curr_stack), s->pivot);
			if (s->curr_pass % 2) { //to Stack B
				fprintf(stderr,"push+rot|");
				push_stack(s->dest_stack, pop_stack(s->curr_stack), dest_partitions[0]);
				rotate_stack(s->dest_stack); /* TODO: wasted move if all same grouping on stack */
			}
			else { //Back to A
				//rot_counter++; /* move bottoms later */
			}
		}
		else //value is large
		{
			if (s->curr_pass % 2) //to Stack B
			{
				fprintf(stderr, "push|");
				push_stack(s->dest_stack, pop_stack(s->curr_stack), dest_partitions[1]);
				rot_counter++;
			}
			else {
			}
		}
		fprintf(stderr, "##(end round)##\n"); fflush(stderr);
		i++;
	}
	while (rot_counter--) {//TODO init rot_counter
		fprintf(stderr, "rotctr--|");
		rotate_stack(s->dest_stack); /* move bottoms */
	}
	print_stacks(s);
	flip_curr_stack(s); /* finished with all stack partitions, next split */
	solve(s);
}

void	solver(t_state *s)
{
	const           t_stack_ptr a = s->stacks[0];
	const int	    bottom_idx = s->nums - 1;;
    t_partition_ptr p;

	if (s->nums < SMALL_NUMS)
		s->passes = 2;
	else if (s->nums < LARGE_NUMS)
		s->passes = 4;
	p = create_partition(a);
	fill_partition(a, p, TOP, bottom_idx);
	solve(s);
	return ;
}

