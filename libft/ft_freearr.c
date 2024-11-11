#include "libft.h"

int	ft_freearr(char **arr)
{
	size_t i;

	i = -1;
	if (arr == NULL)
		return (FAILURE);
	const size_t len = ft_arrlen((const char **)arr);
	while (++i < len)
		free(arr[i]);
	free(arr);
	return (SUCCESS);
}
