#include "pushswap.h"

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
	if (!fill_stack(s->stacks[STACK_A], s->argv))
		return (false);
	fprintf(stderr, "stack filled");
	if (get_partition_id(create_partition(s->stacks[STACK_A])) == 0)
		fprintf(stderr, "partition enumeration correct\n");
	else
		fprintf(stderr, "partition enumeration ERRPR\n");
	s->curr_stack = s->stacks[STACK_A];
	s->dest_stack = s->stacks[STACK_B];
	return (true);
}

void	create_destination_partitions(t_state *s, t_partition_ptr (*d)[2])
{
	t_stack_ptr dest_stack;

	if (s->stacks[0] == s->curr_stack)
		dest_stack = s->stacks[1];
	else
		dest_stack = s->stacks[0];
	fprintf(stderr, "Creating dest parts stack:%d...\n", get_stack_id(dest_stack));
	(*d)[0] = create_partition(dest_stack);
	(*d)[1] = create_partition(dest_stack);
}

void	flip_curr_stack(t_state *s)
{
	if (stack_compare(STACK_A, s->curr_stack) != 0)
		s->curr_stack = s->stacks[STACK_B];

	else
		s->curr_stack = s->stacks[STACK_A];
}

void	print_stacks(t_state *s)
{ 
	fprintf(stderr,"a:");
    print_stack(s->stacks[STACK_A]);
	fprintf(stderr,"\nb:");
    print_stack(s->stacks[STACK_B]);
	fprintf(stderr,"\n");
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
