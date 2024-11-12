#include "pushswap.h"
#include <stdarg.h>

/* Returns true if s1 contains chars from set */
static inline bool	_intersects(char const *s1, char const *set)
{
	char const	*tmp;

	while (*s1)
	{
		tmp = (char *)set;
		while (*tmp)
			if (*s1 == *tmp++)
				return (true);
		s1++;
	}
	return (false);
}

/* Moves words onto array
 * Array may be un-initialized
 */
static int	_realloc_word(char *old, char **new)
{
	if (NULL == old)
		return (FAILURE);
	if (*new)
		free(*new);
	*new = ft_strdup(old);
	if (!*new)
		return (FAILURE);
	return (SUCCESS);
}

/* Returns enlarged array after splitting at i
 * updates argc accordingly
 */
static char	**_realloc_argv(char **argv, size_t i)
{
	char	**arr;
	int		len;
	char	**args;

	if (NULL == argv[i])
		return (NULL);
	arr = ft_split(argv[i], ' ');
	if (!arr)
		return (NULL);
	len = ft_arrlen((const char **)arr);
	args = malloc(sizeof(char *) * (len + 1));
	if (args)
	{
		ft_memset(args, 0, sizeof(char *) * (len + 1));
		i = -1;
		while (++i < (size_t)len)
			if (FAILURE == _realloc_word(arr[i], &(args[i])))
				return (ft_freearr(args), ft_freearr(arr), NULL);
	}
	ft_freearr((char **)arr);
	return (args);
}

static int	_process_arg(char **argv, char ***args, size_t i, int *flag)
{
	if (false == _intersects(argv[i], WHITESPACE))
	{
		if (0 == *flag)
			if (FAILURE == _realloc_word(argv[i], &(*args)[i - 1]))
				return (FAILURE);
	}
	else if (0 == *flag && i == 1)
	{
		free(*args);
		*args = _realloc_argv(argv, i);
		*flag = 1;
		if (!*args)
			return (FAILURE);
	}
	else
		return (FAILURE);
	return (SUCCESS);
}

/* Returns args array and updated argc
 * Check args are all numeric and unique
 * Splits a single shell variable expansion, if whitespace
 * OR just checks args, but not both.
 * args: NULL'd array
 */
int	parse_args(int *argc, char **argv, char ***args)
{
	size_t			i;
	const size_t	tmp = (size_t)(*argc);
	int				flag;

	*args = malloc(sizeof(char *) * (tmp));
	if (!*args)
		return (FAILURE);
	ft_memset(*args, 0, sizeof(char *) * (tmp));
	i = 0;
	flag = 0;
	while (++i < tmp && flag == 0)
	{
		if (FAILURE == _process_arg(argv, args, i, &flag))
			return (FAILURE);
		if (FAILURE == check_word((*args)[i - 1]))
			return (FAILURE);
	}
	if (0 == flag)
	{
		(*args)[i - 1] = NULL;
		*argc = tmp - 1;
	}
	else
		*argc = ft_arrlen(*(const char ***)args);
	return (check_dupes(*args, *argc));
}
