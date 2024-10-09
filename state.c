#include "pushswap.h"

/*  */
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

void	create_destination_partitions(t_state *s, t_partition_ptr (*d)[2])
{
	t_stack_ptr dest_stack;

	if (s->stacks[0] == s->curr_stack)
		dest_stack = s->stacks[1];
	else
		dest_stack = s->stacks[0];
	d[0] = create_partition(dest_stack);
	d[1] = create_partition(dest_stack);
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
}
