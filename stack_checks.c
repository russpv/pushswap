#include "stack_int.h"

bool	is_full(t_stack_ptr s)
{
	if (!s)
		return (false);
	return (s->size == s->max_size);
}

/* Returns true if both are the same */
bool	stack_compare(int a, t_stack_ptr b)
{
	if (a == b->id)
		return (true);
	return (false);
}

bool    is_sorted_asc(t_stack_ptr s)
{
	size_t i;
	const int *idx = s->idx;
    const long    *nums = s->nums; 

	i = 1;
	if (s->size < (size_t)2) {//mydebug("is sorted -- done since size:%zu", s->size);
		return (true);}
	while (i < s->size)
	{
		if (nums[idx[i - 1]] > nums[idx[i]])
			return (false);
		i++;
	}
    mydebug("---- (is_sorted_asc): SORTED!\n");
	return (true);
}

/* Written assuming a single disjoint 'drop' in the sequence */
bool    is_sorted_asc_rotationally(t_stack_ptr s)
{
    int drops = 0;
    int i = -1;

    if (!s)
        return (false);
    while (++i < (int)s->size)
        if (s->nums[s->idx[i]] > s->nums[s->idx[(i + 1) % s->size]])
            drops++;
    if (drops > 1) { //TODO delete this
        mydebug("---- (is sorted rotationally) -- not sorted.\n");
        return (false);
    }
    else
    {
        mydebug("---- (is sorted rotationally) -- SORTED!!\n");
        return (true);
    }
    //return (drops <= 1);
}
