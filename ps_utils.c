#include "pushswap.h"
#include <stdarg.h>

void	err(char *msg, t_state *state)
{
	destroy_state(state);
	ft_printf(msg);
	exit(EXIT_FAILURE);
}

void	printarr(char **arr)
{
	ft_printf("Array:");
	while (*arr)
	{
		ft_printf("%s,", *arr);
		arr++;
	}
	ft_printf("|\n");
}
