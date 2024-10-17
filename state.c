#include "pushswap.h"

# define TOP_IDX 0

void	create_state(t_state **state, char **argv, int argc)
{
	*state = malloc(sizeof(t_state));
	if (!*state)
		err("malloc error", *state);
    (*state)->argv = argv;
	(*state)->nums = (size_t)argc - 1;
	(*state)->print_move = 1;
	(*state)->curr_pass = 0;
	(*state)->curr_stack = NULL;
	(*state)->stacks[STACK_A] = create_stack(STACK_A, (*state)->nums);
	(*state)->stacks[STACK_B] = create_stack(STACK_B, (*state)->nums);
	if (!(*state)->stacks[STACK_A] || !(*state)->stacks[STACK_B])
		err("malloc error", *state);
}

void    destroy_state(t_state *s)
{
    destroy_stack(s->stacks[STACK_A]);
    destroy_stack(s->stacks[STACK_B]);  
    free(s);
}

/* Initially populates stack A with single partition and 
 * sets curr_stack and dest_stack */
bool	fill_stack_a(t_state *s)
{
	const t_stack_ptr		a = s->stacks[STACK_A];
	const int	    		bottom_idx = s->nums - 1;;
	const t_partition_ptr	p = create_partition(a);

	if (!fill_stack(a, s->argv))
		return (false);
	mylog( "State: stack filled\n");
	if (get_partition_id(p) == 0)
		mylog( "Log: partition enumeration correct\n");
	else
		mylog( "Error: partition enumeration ERRPR\n");
	if (!fill_partition(a, p, TOP_IDX, bottom_idx))
		return (false);
	s->curr_stack = s->stacks[STACK_A];
	s->dest_stack = s->stacks[STACK_B];
	mylog( "State: first STACKA part created.\n");
	return (true);
}

void	create_destination_partitions(t_state *s, t_partition_ptr (*d)[2])
{
	t_stack_ptr dest_stack;

	if (s->stacks[0] == s->curr_stack)
		dest_stack = s->stacks[1];
	else
		dest_stack = s->stacks[0];
	mylog( "Log: Creating dest parts stack:%d...\n", get_stack_id(dest_stack));
	(*d)[0] = create_partition(dest_stack);
	(*d)[1] = create_partition(dest_stack);
}

void	flip_curr_stack(t_state *s)
{
	if (stack_compare(STACK_A, s->curr_stack) != 0)
	{
		s->curr_stack = s->stacks[STACK_B];
		s->dest_stack = s->stacks[STACK_A];
	}
	else
	{
		s->curr_stack = s->stacks[STACK_A];
		s->dest_stack = s->stacks[STACK_B];
	}
	mylog( "State: stacks flipped\n");
}

void	print_stacks(t_state *s)
{ 
	mylog("a:");
    print_stack(s->stacks[STACK_A]);
	mylog("\nb:");
    print_stack(s->stacks[STACK_B]);
	mylog("\n");
}

void	rotate_both(t_state *s)
{
	rotate_stack(s->stacks[0]);
    rotate_stack(s->stacks[1]);
}

void	rev_rotate_both(t_state *s)
{
	rev_rotate_stack(s->stacks[0]);
    rev_rotate_stack(s->stacks[1]);
}

bool	is_done(t_state *s)
{
	if (!s)
		err("State not init'd", s);
	if (!s->stacks[0])
		err("Stacks not init'd", s);
	if (is_sorted_asc(s->stacks[0]))
		return (true);
	return (false);
}
