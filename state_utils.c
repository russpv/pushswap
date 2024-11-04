#include "pushswap.h"

void	print_stacks(t_state *s)
{ 
    if (LOGGING == true)
	{
        mylog("a:");
        print_stack(s->stacks[STACK_A]);
	    mylog("\nb:");
        print_stack(s->stacks[STACK_B]);
        mylog("\n");
    }
}

void    print_stack_name(t_stack_ptr s)
{
    if (get_stack_id(s) == STACK_A)
        write(1, "a\n", 2);
    else
        write(1, "b\n", 2);
}
