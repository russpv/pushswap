#include "pushswap.h"

/* Sorts random integers */
// lowest possible estimated number of steps
// get median of stack a
// get median of bottom half of a
// rotate/push all values below median to b
// rotate b such that bottom quartile and top quartile are separated
// repeat until stack a is just 3 numbers
// compute the closest number in stack b, push to a
// rotate stack a

/*  */
static void	_init_stacks(t_state **state, char **argv, int argc)
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

/* Check args are all numeric */
static int	_parse_args(int argc, char **argv)
{
	int i;
	size_t j;

	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (j < ft_strlen(argv[i]))
			if (!ft_isdigit(argv[i][j++]))
				return (FAILURE);
	}
	return (SUCCESS);
}

static void	_solve(t_state *state)
{
	init_stack_a(state);
	if (sorted(state) == FAILURE)
		solver(state);
	return ;
}

int main(int argc, char **argv)
{
	t_state	*state;
	
	state = NULL;
	if (argc < 2)
	{
		ft_printf("Requires numeric arguments to sort");
		return (EXIT_FAILURE);
	}
	if (_parse_args(argc, argv) == FAILURE)
	{
		ft_printf("Bad Input");
		return (EXIT_FAILURE);
	}
	_init_stacks(&state, argv, argc);
	//print_stacks(state);
	//print_arr(state->a, state->a_size);
	_solve(state);
	//print_stacks(state);
	free_struct(state);
	return (EXIT_SUCCESS);
}
