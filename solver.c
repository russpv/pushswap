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
//
//
static void	_flip_stack(char stack)
{
	if (STACK_A == stack)
		return (STACK_B);

	else
		return (STACK_A);
}

// at this point, two stacks have been created ...
// conditionally create a starting partition ...
void	solve(t_state *s)
{
	int rot_counter = 0;
	size_t i = 0;
	int pivot;
	struct s_partition *top_partition;
	struct s_partition **dest_partitions[2];

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
	pivot = get_top_partition_median(s->curr_stack);
	create_destination_partitions(s, dest_partitions);
	while (i < top_partition->size)
	{
		fprintf(stderr, "curr:%ld|", s->a[s->a_idx[0]]);
		print_stack(s->a, s->a_idx, s->a_size); 
		if (peek_top(s->curr_stack) <= pivot) // TODO function ptr refactor
		{
			fprintf(stderr, "%ld below %ld|", s->a[s->a_idx[0]], s->pivot);
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
		fprintf(stderr, "##(end round)##"); fflush(stderr);
		i++;
	}
	while (rot_counter--) {//TODO init rot_counter
		fprintf(stderr, "rotctr--|");
		rotate_stack(s->dest_stack); /* move bottoms */
	}
	s->curr_stack = _flip_stack(s->curr_stack); /* finished with all stack partitions, next split */
	solve(s);
}

void	solver(t_state *s)
{
//TODO get a measure of unsortedness, normalized inversion pairs.
	if (s->nums < 200)
		s->passes = 2; //TODO set this to '2'
	else if (s->nums < 600)
		s->passes = 4;
	solve(s);
	return ;
}

