#include "pushswap.h"
#include <stdarg.h>

void	err(char *msg, t_state *state)
{
	destroy_state(state);
	ft_printf(msg);
	exit(EXIT_FAILURE);
}

/* Returns new clean argv
 * Check args are all numeric 
 */
int	parse_args(int *argc, char **argv, char **args)
{
	int		i;
	size_t	j;

	(void)args;
	i = 0;
	while (++i < *argc)
	{
		j = 0;
		fprintf(stderr, "got:%s", argv[i]);
		while (j < ft_strlen(argv[i]))
		{
			fprintf(stderr, "-%c", argv[i][j]);
			if (!ft_isdigit(argv[i][j]) && argv[i][j] != ' ')
				return (FAILURE);
			j++;
		}
		//args[i] = malloc(sizeof(char*));
		//args[i] = ft_strtrim(argv[i], " ");
	}
	//args[i] = NULL;
	//*argc = i - 1;
	return (SUCCESS);
}