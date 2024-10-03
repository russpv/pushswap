#include "pushswap.h"




void	free_struct(t_state *state)
{
	if (state)
	{
		if (state->a)
		{
			free(state->a);
			free(state->a_idx);
		}
		if (state->b)
		{
			free(state->b);
			free(state->b_idx);
		}
		free(state);
	}
}
