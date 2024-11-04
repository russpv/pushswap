#include "stack_int.h"

/* Checks if n belongs within the interval i-j, accounting
 * for drop point in the sort 
 * if i > j, this is a drop point
 */
static bool  is_between(t_stack_ptr s, long n, int i, int j)
{
    const long l = get_stack_num(s, get_stack_idx_at_depth(s, i));
    const long r = get_stack_num(s, get_stack_idx_at_depth(s, j));

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

/* returns index (or # rotates) of position that maintains ascending sort
 * order. assumes sorted stack. binary search. 
 * case 1: array can be descending order for size=2, so special case handled.
 * case 2: returns 0 for a new max num on a strictly asc stack 
 */
int get_insert_position(t_stack_ptr s, long num)
{
    int high;
    int low;
    int mid;

    high = s->size - 1;
    low = 0;
    if (s->size == 0 || s->size == 1)
        return (0);
    if (s->size == 2)
    {
        if (s->nums[s->idx[low]] < s->nums[s->idx[high]])
        {
            if (num > s->nums[s->idx[low]] && num < s->nums[s->idx[high]])
            {
                //mydebug("---- (binary search) -- s2, asc, inbetw, 1\n");
                return (1);
            }
            else {
                //mydebug("---- (binary search) -- s2, asc, outsi, 0\n");
                return (0);
            }
        }
        else /* descending*/
        {
            if (num < s->nums[s->idx[low]] && num > s->nums[s->idx[high]]){
                //mydebug("---- (binary search) -- s2, DESC, inbetw, 0\n");
                return (0);
            } else {
                //mydebug("---- (binary search) -- s2, DESC, outsi, 1\n");
                return (1);
            }
        }
    }
    while (low < high)
    {
        mid = low + (high - low) / 2; 
        if ( is_between(s, num, mid - 1, mid) ) {
                mydebug("---- (binary search) -- %d goes at i:%d betw i-1:%ld and i:%ld\n", \
                        num, mid, get_stack_num(s, get_stack_idx_at_depth(s, mid - 1)), get_stack_num(s, mid));
                high = mid;
                break ;
                }
        /* left is sorted (right may also be)*/
        if (s->nums[s->idx[low]] <= s->nums[s->idx[mid]])
        {
            if ((!is_between(s, num, low - 1, mid))) /* go to other portion */
            {
                //mydebug("---- (binary search) -- ls, r: l = m + 1; num:%ld l[%d] m[%d] h[%d]\n",\
                //        num, low, mid, high);
                low = mid + 1;
            }
            else /* within left portion  */
            {
                //mydebug("---- (binary search) -- ls, l: h = m; num:%ld l[%d], m[%d] h[%d]\n",\
                //        num, low, mid, high);
                high = mid; /* we never test high, just compute mid and terminate  */
            }
        }
        else /* right is sorted */
        {
            if (!is_between(s, num, mid - 1, high))  { /* go to other portion */
                //mydebug("---- (binary search) -- rs, l: h = m; num:%ld l[%d] m[%d] h[%d]\n",\
                //        num, low, mid, high);
                high = mid; // mid could still be correct, not +1
            } else { /* within right portion */
                //mydebug("---- (binary search) -- rs, r: l = m + 1; num:%ld l[%d] m[%d] h[%d]\n",\
                //        num, low, mid, high);
                low = mid + 1;
            }
        }
    }
    mydebug("---- (binary search) -- returning %d; l_%d m_%d h_%d\n", high, low, mid, high);
    return (high);
}
