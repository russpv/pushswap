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
	if (i < get_middle_idx(s)) /* is the top idx */
		return (i);
	else
		return (-1 * ((int)size - i)); /* inclusive */
}

// TODO parse this function and doc it
/* Returns median of numbers on top of stack */
int	get_median(long *nums, int *idx, size_t size)
{
	int flag;
	long tmp;
	size_t i;
	long arr[size];

	i = 0;
	while (i < size)
	{
		arr[i] = nums[idx[i]];
		i++;
	}
	if (size == 1)
		return (arr[0]);
	while (1)
	{
		i = 0;
		flag = 0;
		while (++i < size)
		{
			if (arr[i] < arr[i - 1]) // sort ascending rightward
			{
				tmp = arr[i];
				arr[i] = arr[i - 1];
				arr[i - 1] = tmp;
				flag = 1;
			}
		}
		if (flag == 0)
			break ;
	}
	if (size % 2) // odd
	{
		mydebug("Log: median:%ld\n", arr[size / 2]);
		return (arr[size / 2]);
	}
	mydebug("---- (get_median): median:%ld\n", arr[size / 2 - 1]);
	return (arr[size / 2 - 1]);
}