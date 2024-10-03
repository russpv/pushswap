#include "pushswap.h"

static int		_init(void	**arr, size_t size, size_t type_size)
{
	*arr = malloc(type_size * (size));
	if (!*arr)
		return (FAILURE);
	return (SUCCESS);
}

void	init_stack_a(t_state *state)
{
	size_t i;

	if ((_init((void**)&state->a, state->nums, sizeof(long)) == FAILURE) || 
		(_init((void**)&state->b, state->nums, sizeof(long)) == FAILURE) || 
		(_init((void**)&state->a_idx, state->nums, sizeof(size_t)) == FAILURE) || 
		(_init((void**)&state->b_idx, state->nums, sizeof(size_t)) == FAILURE) )
	{
		err("Malloc error", state);
		return ;
	}
	i = 0;
	while (i < state->nums)
	{
		state->a[i] = (long)ft_atoi(state->argv[i + 1]);
		state->a_idx[i] = i;
		state->b[i] = 0;
		state->b_idx[i] = 0;
		i++;
	}
	state->a_size = state->nums;
	state->b_size = 0;
	return ;
}
