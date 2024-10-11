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
	fill_stack_a(state);
	print_stacks(state);
	fprintf(stderr, "Main: ready to solve\n");
	if (is_done(state) == false)
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
	create_state(&state, argv, argc);
	//print_arr(state->a, state->a_size);
	_solve(state);
	//print_stacks(state);
	destroy_state(state);
	return (EXIT_SUCCESS);
}
