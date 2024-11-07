#include "../pushswap.h"







int main (int argc, char *argv[])
{
	t_state	*state;

	state = NULL;
	if (argc < 2)
	{
		ft_printf("Requires numeric arguments to check");
		return (EXIT_FAILURE);
	}
	if (parse_args(argc, argv) == FAILURE)
	{
		ft_printf("Bad Input");
		return (EXIT_FAILURE);
	}
	create_state(&state, argv, argc);
	ft_printf("Solution goes here!");
	destroy_state(state);
	return (EXIT_SUCCESS);

}
