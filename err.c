#include "pushswap.h"

void	err(char *msg, t_state *state)
{
	free_struct(state);
	ft_printf(msg);
	exit(EXIT_FAILURE);
}
