#include "libft.h"

int	ft_freearr(char **arr)
{
	size_t i;

	i = -1;
	if (arr == NULL)
		return (0);
	const size_t size = ft_arrlen((const char **)arr);
	while (++i < size)
		free(arr[i]);
	free(arr);
	return (1);
}