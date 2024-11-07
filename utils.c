#include "pushswap.h"
#include <stdarg.h>

void	err(char *msg, t_state *state)
{
	destroy_state(state);
	ft_printf(msg);
	exit(EXIT_FAILURE);
}

/* Check args are all numeric */
int	parse_args(int argc, char **argv)
{
	int		i;
	size_t	j;

	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (j < ft_strlen(argv[i]))
		{
			if (!ft_isdigit(argv[i][j]) && argv[i][j] != ' ')
				return (FAILURE);
			j++;
		}
	}
	return (SUCCESS);
}