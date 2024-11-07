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

static int	_realloc_word(char *old, char **new)
{
	if (new)
		if (*new)
			free(*new);
	*new = ft_strdup(old);
	if (!*new)
		return (FAILURE);
	return (SUCCESS);
}

/* Returns enlarged array after splitting at i 
 * Preserves dummy position of program name
 */
static char	**_realloc_argv(int *argc, char **argv, size_t i)
{
	char	**arr;
	int		len;
	char	**new_argv;

	arr = ft_split(argv[i], ' ');
	if (!arr)
		return (NULL);
	len = ft_arrlen((const char **)arr) + 1;
	new_argv = malloc(sizeof(char *) * (len + 2));
	if (new_argv)
	{
		new_argv[0] = malloc(sizeof(char) * 1);
		if (!new_argv[0])
			return (ft_freearr(new_argv), NULL);
		new_argv[0][0] = 0;
		i = 0;
		*argc = len;
		while (++i < (size_t)(*argc))
			if (FAILURE == _realloc_word(arr[i - 1], &(new_argv[i])))
				return (ft_freearr(new_argv), NULL);
		new_argv[i] = NULL;
	}
	ft_freearr((char **)arr);
	return (new_argv);
}

static int	_check_word(char *word)
{
	size_t	letter;

	letter = 0;
	while (letter < ft_strlen(word))
	{
		if (!ft_isdigit(word[letter]))
			return (FAILURE);
		letter++;
	}
	return (SUCCESS);
}

/* Returns new clean argv and modifies argc
 * Check args are all numeric
 * Splits a single shell variable expansion
 * args: NULL'd array
 */
int	parse_args(int *argc, char **argv, char ***args)
{
	size_t	i;
	size_t	tmp;

	printarr(argv);
	tmp = (size_t)(*argc);
	i = 0;
	while (++i < tmp)
	{
		if (false == _intersects(argv[i], WHITESPACE))
		{
			if (FAILURE == _realloc_word(argv[i], args[i]))
				return (FAILURE);
		}
		else if (i == 1)
		{
			*args = _realloc_argv(argc, argv, i);
			tmp = 1;
			if (!*args)
				return (FAILURE);
		}
		else
			return (FAILURE);
		if (FAILURE == _check_word((*args)[i]))
			return (FAILURE);
	}
	return (SUCCESS);
}
