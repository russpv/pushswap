#include "pushswap.h"

#define PASSES 2

// Debug statements to add:
// 		mylog("Doing greedy sort.\n");
/*		mylog("####### PASS %d, LOOP %zu ##\n", s->curr_pass, j);
		mylog("Solve: top partition:%p size:%zu id:%d value:%ld\n",
			curr_partition, get_partition_size(curr_partition),
			get_partition_id(curr_partition), peek_partition(curr_partition));*/
/*		mylog("Solve: median value %ld\n", s->pivot); */
static void	_solve(t_state *s)
{
	const t_partition_ptr	top_partition = get_top_partition(s->curr_stack);
	const size_t			partitions = get_partition_count(s->curr_stack);
	size_t					j;

	j = -1;
	mylog("\n########## PASS_%d ##########\n", ++s->curr_pass);
	if (s->curr_pass == PASSES)
	{
		if (greedy_sort(s) == false)
			err("Greedy sort failed.\n", s);
		return ;
	}
	if (is_sorted_asc(s->stacks[STACK_A]) && is_full(s->stacks[STACK_A]))
		return (mylog("Stack A is sorted!! Returning...\n"));
	while (++j < partitions)
	{
		if (!get_top_partition(s->curr_stack))
			err("ERR: partition get error", s);
		s->pivot = get_top_partition_median(s->curr_stack);
		quicksort_partition(s, top_partition, get_top_partition(s->curr_stack),
			get_partition_size(get_top_partition(s->curr_stack)));
	}
	flip_curr_stack(s);
	_solve(s);
}

void	solver(t_state *s)
{
	fill_stack_a(s);
	print_stacks(s);
	s->passes = PASSES;
	if (!is_done(s))
		_solve(s);
	return ;
}
