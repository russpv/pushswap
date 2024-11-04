#include "pushswap.h"

// This is quicksort.c

void	process_partition(t_state *s, t_partition_ptr top_partition,
		t_partition_ptr partition, size_t limit)
{
	size_t i = 0;
	int rot_counter = 0;
	t_partition_ptr dest_partitions[2];
	create_destination_partitions(s, &dest_partitions);
	while (i < limit)
	{
		mylog("Solve: %zu loop started (%zu moves remaining p:%d, %zu p's)\n",
			i, get_partition_size(partition), get_partition_id(partition),
			get_partition_count(s->curr_stack));
		print_stacks(s);
		// swap_both_if_needed(s);
		if (peek_stack(s->curr_stack) <= s->pivot)
		{
			if (peek_next_stack(s->curr_stack) <= s->pivot
				&& get_stack_size(s->curr_stack) > 2)
				// swap_stack_if_needed(s);
				mylog("Solve: %ld below median %ld\n",
					peek_stack(s->curr_stack), s->pivot);
			mylog("Solve: push lower\n");
			move(s->dest_stack, PUSH, pop_stack(s->curr_stack),
				dest_partitions[0], PRINT_ON);
			print_stacks(s);
			// to STACKB, largers stay on top
			if (s->dest_stack == s->stacks[STACK_B])
			{
				if (partition == top_partition) /* if first partition sorting,
					only rotate */
				{
					mylog("Solve: rot\n");
					move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
						/* TODO: wasted move if all same grouping on stack */
				}                                                  
					/* if not top partition, don't rotate smallers */
			}
			// back to STACKA, smallers go to bottom temporarily
			else if (s->dest_stack == s->stacks[STACK_A]
				&& partition != top_partition)
			{ /* never an empty stack, except first partition sorting */
				mylog("Solve: rot + banking revrot\n");
				move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
				rot_counter++;
			}
		}
		else // value is large
		{
			if (peek_next_stack(s->curr_stack) > s->pivot
				&& get_stack_size(s->curr_stack) > 2)
				// swap_stack_if_needed(s);
				mylog("Solve: push larger\n");
			move(s->dest_stack, PUSH, pop_stack(s->curr_stack),
				dest_partitions[1], PRINT_ON);
			if (s->dest_stack == s->stacks[STACK_B]) // largers stay on top,
				so temporarily go bottom
			{
				if (partition != top_partition) /* if not top partition,
					rotate largers and reverse rotate later */
				{
					mylog("Solve: rot + reverse later\n");
					move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
					rot_counter++;
				} /* if top partition, don't rotate */
			}
			else if (s->dest_stack == s->stacks[STACK_A]
				&& partition == top_partition)
			{
				mylog("Solve: rot\n");
				move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
			}
		}
		print_stacks(s);
		mylog("##(end round)##\n\n");
		fflush(stderr);
		i++;
	}
	while (rot_counter-- > 0)
	{
		mylog("Solve: rot counter rot\n");
		move(s->dest_stack, REV_ROTATE, 0, NULL, PRINT_ON); // move bottoms
		print_stacks(s);
	}
}