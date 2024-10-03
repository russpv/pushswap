#include "pushswap.h"

int	sorted(t_state *state)
{
	size_t i;
	size_t *idx = state->a_idx;
	long *a = state->a;

	i = 1;
	if (state->a_size < 2)
		return (SUCCESS);
	while (i < state->a_size)
	{
		if (a[idx[i - 1]] > a[idx[i]])
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}
