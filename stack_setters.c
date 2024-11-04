#include "stack_int.h"

static bool   _update_stack_extremum(t_stack_ptr s)
{
    if (s->size == 0)
        return (false);
    s->min = LONG_MAX;
    s->max = LONG_MIN;
    long testnum;
    int i = -1;

    while (++i < (int)s->size)
    {
        testnum = get_stack_num(s, i);
        if (testnum < s->min)
            s->min = testnum;
        if (testnum > s->max)
            s->max = testnum;
    }
    return (true);
}

/* Tests if num is beyond stack min or max */
bool    is_stack_extremum(t_stack_ptr s, long num)
{
    if (s->last_update_size != s->size)
        if (_update_stack_extremum(s) == false)
            return (false);
    if (num < s->min || num > s->max)
        return (true);
    return (false);
}
