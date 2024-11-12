#include "pushswap.h"

#define PASSES 2

/* Takes an unsorted stack A of three nums and sorts it */
static void	_three_solve(t_state *s)
{
	const t_stack_ptr	st = s->stacks[STACK_A];

	if (3 != get_stack_size(st))
		return ;
	if (get_stack_num(st, 0) > get_stack_num(st, 1))
		move(st, SWAP, 0, NULL, PRINT_ON);
	if (get_stack_num(st, 2) < get_stack_num(st, 0))
		move(st, REV_ROTATE, 0, NULL, PRINT_ON);
	else
	{
		if (is_sorted_asc(st))
		{
			(mylog("three_solve: Stack A is sorted!! \n"));
			return ;
		}
		move(st, REV_ROTATE, 0, NULL, PRINT_ON);
		move(st, SWAP, 0, NULL, PRINT_ON);
	}
	if (is_sorted_asc(st))
	{
		mylog("three_solve: Stack A is sorted!! \n");
		return ;
	}
	else
		err("ERR Three solve\n", s);
}

/* Takes an unsorted stack A of five nums and sorts it */
static void	_five_solve(t_state *s)
{
	const t_stack_ptr		st = s->stacks[STACK_A];
	const t_partition_ptr	p_b = create_partition(s->stacks[STACK_B]);

	if (5 != get_stack_size(st))
		return ;
	while (get_stack_size(st) > 3)
	{
		mylog("St size:%zu\n", get_stack_size(st));
		move(s->stacks[STACK_B], PUSH, pop_stack(st), p_b, PRINT_ON);
	}
	mylog("Calling three_solve\n");
	_three_solve(s);
	mylog("Calling greedy_sort\n");
	if (false == greedy_sort(s))
		err("ERR greedy sort failed\n", s);
	if (is_sorted_asc(s->stacks[STACK_A]) && is_full(s->stacks[STACK_A]))
	{
		mylog("five_solve: Stack A is sorted!! Returning...\n");
		return ;
	}
}

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
	if (is_done(s))
		return ;
	if (3 == s->nums)
		_three_solve(s);
	else if (5 == s->nums)
		_five_solve(s);
	else
		_solve(s);
	return ;
}
