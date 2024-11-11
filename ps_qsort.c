#include "pushswap.h"

// Debug statement to insert at first line:
/*	if (peek_next_stack(s->curr_stack) > s->pivot
		&& get_stack_size(s->curr_stack) > 2)
		mylog("Solve: push larger\n");*/
static bool	_do_larger(t_state *s, t_partition_ptr partition,
		t_partition_ptr top_partition, t_partition_ptr dest_partitions[])
{
	move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[1], \
		PRINT_ON);
	if (s->dest_stack == s->stacks[STACK_B])
	{
		if (partition != top_partition)
		{
			move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
			return (true);
		}
	}
	else if (s->dest_stack == s->stacks[STACK_A] && partition == top_partition)
		move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
	return (false);
}

// Debug statement to insert at first line:
/*	if (peek_next_stack(s->curr_stack) <= s->pivot
		&& get_stack_size(s->curr_stack) > 2)
		mylog("Solve: %ld below median %ld\n",
			peek_stack(s->curr_stack), s->pivot); */
static bool	_do_smaller(t_state *s, t_partition_ptr partition,
		t_partition_ptr top_partition, t_partition_ptr dest_partitions[])
{
	move(s->dest_stack, PUSH, pop_stack(s->curr_stack), dest_partitions[0], \
		PRINT_ON);
	if (s->dest_stack == s->stacks[STACK_B])
	{
		if (partition == top_partition)
			move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
	}
	else if (s->dest_stack == s->stacks[STACK_A] && partition != top_partition)
	{
		move(s->dest_stack, ROTATE, 0, NULL, PRINT_ON);
		return (true);
	}
	return (false);
}

/*
 * Stack B is sorted high to low.
 */
// Debug statements to add within while loop:
/*		mylog("## Solve: %zu loop started (%zu moves remaining p:%d, \
			%zu p's)\n", i, get_partition_size(partition), \
			get_partition_id(partition), get_partition_count(s->curr_stack));*/
/*		mylog("## Solve: end of round.\n\n"); */
void	quicksort_partition(t_state *s, t_partition_ptr top_partn,
		t_partition_ptr partn, size_t limit)
{
	size_t			i;
	int				rot_counter;
	t_partition_ptr	dest_partns[2];

	i = -1;
	rot_counter = 0;
	create_destination_partitions(s, &dest_partns);
	while (++i < limit)
	{
		if (peek_stack(s->curr_stack) <= s->pivot)
		{
			if (true == _do_smaller(s, top_partn, partn, dest_partns))
				rot_counter++;
		}
		else if (true == _do_larger(s, top_partn, partn, dest_partns))
			rot_counter++;
		print_stacks(s);
	}
	while (rot_counter-- > 0)
		move(s->dest_stack, REV_ROTATE, 0, NULL, PRINT_ON);
	print_stacks(s);
}
