#include "stack_int.h"

/* Returns lower middle index for even size */
int	get_middle_idx(t_stack_ptr s)
{
	return (get_stack_size(s) / 2);
}

/* Returns number of rotates or reverse rotates */
int	get_moves_back_to_head(t_stack_ptr s)
{
	const size_t	size = get_stack_size(s);
	int				i;

	i = 0;
	if (size == 1)
		return (0);
	if (is_sorted_asc(s))
		return (0);
	if (size == 2)
		return (1);
	while (++i < (int)size)
		if (!(get_stack_num(s, i - 1) < get_stack_num(s, i)))
			break ;
	if (i < get_middle_idx(s))
		return (i);
	else
		return (-1 * ((int)size - i));
}

static int	_bubble_sort(long arr[], size_t size)
{
	long	tmp;
	size_t	i;
	int		swaps;

	i = 0;
	swaps = 0;
	while (++i < size)
	{
		if (arr[i] < arr[i - 1])
		{
			tmp = arr[i];
			arr[i] = arr[i - 1];
			arr[i - 1] = tmp;
			swaps = 1;
		}
	}
	return (swaps);
}

/* Returns median of size numbers from top of stack
 * Uses bubble sort
 */
int	get_median(long *nums, int *idx, size_t size)
{
	long	arr[MAX_SIZE];
	size_t	i;

	i = -1;
	while (++i < size)
		arr[i] = nums[idx[i]];
	if (size == 1)
		return (arr[0]);
	while (1)
	{
		if (_bubble_sort(arr, size) == 0)
			break ;
	}
	if (size % 2)
	{
		if (true == DEBUGGING)
			mydebug("---- (get_median) -- %d\n", arr[size / 2]);
		return (arr[size / 2]);
	}
	if (true == DEBUGGING)
		mydebug("---- (get_median) -- %d\n", arr[size / 2 - 1]);
	return (arr[size / 2 - 1]);
}
