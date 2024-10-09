#include "pushswap.h"

void	create_destination_partitions(t_state *s, struct s_partition (*)d[2])
{
	t_stack_ptr dest_stack;

	if (s->stacks[0] == s->curr_stack)
		dest_stack = s->stacks[1];
	else
		dest_stack = s->stacks[0];
	d[0] = create_partition(dest_stack);
	d[1] = create_partition(dest_stack);
}
