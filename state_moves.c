#include "pushswap.h"

void    push(t_stack_ptr s, va_list args)
{
    const long num = va_arg(args, long);
    const t_partition_ptr partition = va_arg(args, t_partition_ptr);
    const int   print = va_arg(args, int); 
    if (!s)
        exit(1);
    if (push_stack(s, num, partition) == false)
        exit(1);
    if (print == PRINT_OFF)
        return ;
    write(1, "p", 1);
    print_stack_name(s);
}

void    rotate(t_stack_ptr s, va_list args)
{
    va_arg(args, long);
    va_arg(args, t_partition_ptr);
    const int   print = va_arg(args, int);
    if (rotate_stack(s) == false)
        exit(1);
    if (print == PRINT_OFF)
        return ;
    write(1, "r", 1);
    print_stack_name(s);
}

void    rev_rotate(t_stack_ptr s, va_list args)
{
    va_arg(args, long);
    va_arg(args, t_partition_ptr);
    const int   print = va_arg(args, int);
    if (rev_rotate_stack(s) == false)
    {
        mydebug("Invalid move, no bueno...\n");
        exit(1);
    }
    if (print == PRINT_OFF)
        return ;
    write(1, "rr", 2);
    print_stack_name(s);
}

void    swap(t_stack_ptr s, va_list args)
{
    va_arg(args, long);
    va_arg(args, t_partition_ptr);
    const int print = va_arg(args, int);
    if (swap_stack(s) == false)
        exit(1);
    if (print == PRINT_OFF)
        return ;
    write(1, "s", 1);
    print_stack_name(s);
}

void    move(t_stack_ptr s, enum e_move_type move, ...)
{
    const t_move jumptable[MOVE_COUNT] = {push, \
        rotate, rev_rotate, swap};
    va_list args;

    va_start(args, move);
    if (move >= MOVE_COUNT || move < 0)
        ft_printf("Invalid move\n");
    jumptable[move](s, args);
    va_end(args);
}