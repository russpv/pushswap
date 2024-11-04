#include "stack_int.h"

long	peek_partition(t_partition_ptr p)
{
	return (peek_stack(p->stack));
}
