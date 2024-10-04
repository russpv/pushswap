#include "pushswap.h"

void	print_stacks(t_state *s)
{ 
	size_t i;

	i = 0;
	fprintf(stderr,"a:");
	while (i < s->a_size)
		fprintf(stderr,"%ld ", s->a[s->a_idx[i++]]);	
	fflush(stderr);
	i = 0;
	fprintf(stderr,"\nb:");
	while (i < s->b_size)
		fprintf(stderr,"%ld ", s->b[s->b_idx[i++]]);
	fflush(stderr);
}

void	print_stack(long *arr, size_t *idx, size_t size)
{
	if (0 == size)
		return ;
	for (size_t j = 0; j < size - 1; j++)
		fprintf(stderr, " %ld", arr[idx[j]]);
	fprintf(stderr,"|");
}
static void	_print_arr(long *arr, size_t size)
{
	for (size_t j = 0; j < size - 1; j++)
	{
		fprintf(stderr, " %ld", arr[j]);
	}
}

/* Simple bubble sort */
int		get_median(long *nums, size_t *idx, size_t size)
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
	_print_arr(arr, size);
	if (size % 2) //odd
	{
		fprintf(stderr, "median:%ld", arr[size / 2]);
		return (arr[size / 2]);
	}
	fprintf(stderr, "median:%ld", arr[size / 2 -1 ]);
	return (arr[size / 2 - 1]);
}
