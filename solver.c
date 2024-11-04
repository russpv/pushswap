#include "pushswap.h"

// create two more arrays that store '1' where a x-tile boundary is
// calculate the median somewhere
// if above median if !flipped push, else rotate then push + flipped
// you are left with a x-tile at top of stack from a prior sort
// rotate the destination stack to correct ascending order
// repeat on that x-tile and push to opposite stack until no more on current stack
// bubble sort swap each x-tile on stack a to finish NO - rotate+swap,
//	might as well push
// insertion sort

// figure out how to manage how many passes to do
// if n>500, n-2n per divide round k, n/(2^k)-n/(2^k)*(n/2) per insertion round
//	* x-tiles
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
// Second pass do a: 33 | 44 || 1111,
//	22 | 33 | 44 | 11 >> 11 | 22 | 33 | 44; rightside
// Third pass do b: 8 | 7
//	|| 33 | 22 | 11 >> 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1; inverted
// Fourth pass do a:  rightside

#define PASSES 2

void	solve(t_state *s)
{
	const t_partition_ptr	top_partition = get_top_partition(s->curr_stack);
	const size_t			partitions = get_partition_count(s->curr_stack);
	size_t					j;
	t_partition_ptr			curr_partition;

	j = 0;
	++s->curr_pass;
	mylog("\n########## PASS_%d ##########\n", s->curr_pass);
	if (s->curr_pass == PASSES)
	{
		mylog("Doing greedy sort.\n");
		if (greedy_sort(s) == false)
			err("Greedy sort failed.\n", s);
		return ;
	}
	if (is_sorted_asc(s->stacks[STACK_A]) && is_full(s->stacks[STACK_A]))
	{
		mylog("Stack A is sorted!! Returning...\n");
		return ;
	}
	while (j < partitions)
	{
		curr_partition = get_top_partition(s->curr_stack);
		mylog("####### PASS %d, LOOP %zu ##\n", s->curr_pass, j);
		mylog("Solve: top partition:%p size:%zu id:%d value:%ld\n",
			curr_partition, get_partition_size(curr_partition),
			get_partition_id(curr_partition), peek_partition(curr_partition));
		if (!curr_partition)
			err("ERR: partition get error", s);
		s->pivot = get_top_partition_median(s->curr_stack);
		mylog("Solve: median value %ld\n", s->pivot);
		process_partition(s, top_partition, curr_partition,
			get_partition_size(curr_partition));
		j++;
	}
	print_stacks(s);
	flip_curr_stack(s);
	solve(s);
}

void	solver(t_state *s)
{
	fill_stack_a(s);
	print_stacks(s);
	s->passes = PASSES;
	if (!is_done(s))
		solve(s);
	return ;
}

/*
void	swap_both_if_needed(t_state *s)
{
	if (s->curr_stack == s->stacks[STACK_A])
	{
		if ((get_stack_size(s->dest_stack) >= 2) && \
				(get_stack_size(s->curr_stack) >= 2) && \
				(peek_stack(s->curr_stack) > peek_next_stack(s->curr_stack))
					&& \
				(peek_stack(s->dest_stack) < peek_next_stack(s->dest_stack)))
		{
			mylog("Solve: swapping both\n");
			swap_both(s);
		}
	} else {
		if ((get_stack_size(s->dest_stack) >= 2) && \
				(get_stack_size(s->curr_stack) >=2 ) && \
				(peek_stack(s->curr_stack) < peek_next_stack(s->curr_stack))
					&& \
				(peek_stack(s->dest_stack) > peek_next_stack(s->dest_stack)))
		{
			mylog("Solve: swapping both\n");
				swap_both(s);
		}
	}
}*/
/*
void	swap_stack_if_needed(t_state *s)
{
	if (s->dest_stack == s->stacks[STACK_A] && \
		(peek_stack(s->curr_stack) < peek_next_stack(s->curr_stack)))
		move(s->curr_stack, SWAP, 0, NULL, PRINT_ON);
	else if (s->dest_stack == s->stacks[STACK_B] && \
			(peek_stack(s->curr_stack) > peek_next_stack(s->curr_stack)))
		move(s->curr_stack, SWAP, 0, NULL, PRINT_ON);
}*/
