#include "stack_int.h"

/* Checks if n belongs within the interval i-j, accounting
 * for drop point in the sort
 * if i > j, this is a drop point
 */
static bool	_is_between(t_stack_ptr s, long n, int i, int j)
{
	const long	l = get_stack_num(s, get_stack_idx_at_depth(s, i));
	const long	r = get_stack_num(s, get_stack_idx_at_depth(s, j));

	if (l == LONG_MAX || r == LONG_MAX)
		return (false);
	if (l == r)
		return (true);
	if (l > r)
		if (n < r || n > l)
			return (true);
	if (n > l && n < r)
		return (true);
	return (false);
}

// Debug statements to add:
// mydebug("---- (binary search) -- s2, asc, inbetw, 1\n");
// mydebug("---- (binary search) -- s2, asc, outsi, 0\n");
// mydebug("---- (binary search) -- s2, DESC, inbetw, 0\n");
// mydebug("---- (binary search) -- s2, DESC, outsi, 1\n");
static int	_binary_search_size_two(t_stack_ptr s, int low, int high)
{
	if (s->nums[s->idx[low]] < s->nums[s->idx[high]])
	{
		if (s->tmp > s->nums[s->idx[low]] && s->tmp < s->nums[s->idx[high]])
			return (1);
		else
			return (0);
	}
	else
	{
		if (s->tmp < s->nums[s->idx[low]] && s->tmp > s->nums[s->idx[high]])
			return (0);
		else
			return (1);
	}
}

static void	_left_sorted_tests(t_stack_ptr s, int *low, int mid, int *high)
{
	if ((!_is_between(s, s->tmp, *low - 1, mid)))
		*low = mid + 1;
	else
		*high = mid;
}

static void	_right_sorted_tests(t_stack_ptr s, int *low, int mid, int *high)
{
	if (!_is_between(s, s->tmp, mid - 1, *high))
		*high = mid;
	else
		*low = mid + 1;
}

/* returns index (or # rotates) of position that maintains ascending sort
 * order. assumes sorted stack. binary search.
 * case 1: array can be descending order for size=2, so special case handled.
 * case 2: returns 0 for a new max num on a strictly asc stack
 */
// Debug statements to add:
/* left is sorted (right may also be)*/
/* go to other portion
mydebug("---- (binary search) -- ls, r: l = m \
   + 1; num:%ld l[%d] m[%d] h[%d]\n",\
		num, low, mid, high); */
/* within left portion
mydebug("---- (binary search) -- ls, l: h = m; num:%ld l[%d],
	m[%d] h[%d]\n",\
		num, low, mid, high); */
/* right is sorted */
/* go to other portion
mydebug("---- (binary search) -- rs,
	l: h = m; num:%ld l[%d] m[%d] h[%d]\n",\
		num, low, mid, high); */
/* within right portion
mydebug("---- (binary search) -- rs, r: l = m \
	+ 1; num:%ld l[%d] m[%d] h[%d]\n",\
		num, low, mid, high); */
/* mydebug("---- (binary search) -- returning %d; l_%d m_%d h_%d\n", high,
	low, mid, high);*/
// Return statement:
/*mydebug("---- (binary search) -- %d goes at i:%d \
			betw i-1:%ld and i:%ld\n", num, mid, get_stack_num(s, \
			get_stack_idx_at_depth(s, mid - 1)), get_stack_num(s, mid));*/
int	get_insert_position(t_stack_ptr s, long num)
{
	int	high;
	int	low;
	int	mid;

	high = s->size - 1;
	low = 0;
	s->tmp = num;
	if (s->size == 0 || s->size == 1)
		return (0);
	if (s->size == 2)
		return (_binary_search_size_two(s, low, high));
	while (low < high)
	{
		mid = low + (high - low) / 2;
		if (_is_between(s, num, mid - 1, mid))
		{
			high = mid;
			break ;
		}
		if (s->nums[s->idx[low]] <= s->nums[s->idx[mid]])
			_left_sorted_tests(s, &low, mid, &high);
		else
			_right_sorted_tests(s, &low, mid, &high);
	}
	return (high);
}
