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

int	main(int argc, char **argv)
{
	t_state	*state;

	state = NULL;
	if (argc < 2)
		return (EXIT_FAILURE);
	if (parse_args(argc, argv) == FAILURE)
	{
		ft_printf("Error");
		return (EXIT_FAILURE);
	}
	create_state(&state, argv, argc);
	solver(state);
	destroy_state(state);
	return (EXIT_SUCCESS);
}
