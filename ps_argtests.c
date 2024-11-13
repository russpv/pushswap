#include "pushswap.h"

/* Interface for hashtable */
int	check_dupes(char **args, size_t size)
{
	size_t	i;

	i = -1;
	if (NULL == args || 0 == size)
		return (FAILURE);
	while (++i < size)
	{
		if (NULL == lookup(args[i]))
			install(args[i], NULL);
		else
		{
			destroy_hasht();
			return (FAILURE);
		}
	}
	destroy_hasht();
	return (SUCCESS);
}

/* Tests if word is an integer no space */
int	check_word(char *word)
{
	size_t	i;
	long	num;

	i = -1;
	while (word[++i])
	{
		if (i == 0 && word[i] == '-')
			continue ;
		else if (!ft_isdigit(word[i]))
			return (FAILURE);
	}
	num = ft_atol(word);
	if (num > INT_MAX || num < INT_MIN)
		return (FAILURE);
	return (SUCCESS);
}

int	has_digits(char *word)
{
	size_t	i;
	int		flag;

	i = -1;
	flag = 0;
	while (word[++i])
	{
		if (ft_isdigit(word[i]))
			flag = 1;
	}
	if (1 == flag)
		return (SUCCESS);
	return (FAILURE);
}
