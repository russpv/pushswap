#include "stack_int.h"

bool    increment_partition(t_stack_ptr s, int idx)
{
    if (NULL == s->partitions[idx])
        return (false);
    s->partitions[idx]->size++;
    return (true);
}

bool    decrement_partition(t_stack_ptr s, int idx)
{
    if (NULL == s->partitions[idx])
        return (false);
    if (1 == s->partitions[idx]->size)
    {
        s->partitions[idx]->size--;
        s->partition_count--;
        destroy_partition(&s->partitions[idx]);
    }
    else
        s->partitions[idx]->size--;
    return (true);
}