#include "libft.h"

/* Returns number of strings in a null-
 * terminated array of arrays.
 * Excludes NULL.
 */
size_t	ft_arrlen(const char**arr)
{
	size_t count = 0;

	while (arr[count] != NULL)
		count++;
	return (count);
}
