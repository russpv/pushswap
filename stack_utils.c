#include "stack_int.h"

void	mydebug(const char *format, ...)
{
	int	original_stdout;
		va_list args;

	if (DEBUGGING == true)
	{
		original_stdout = dup(STDOUT_FILENO);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		ft_printf("%s", GREY);
		va_start(args, format);
		ft_printf(format, args);
		va_end(args);
		ft_printf("%s", RESET);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}

void	print_stack(t_stack_ptr stack)
{
	const size_t size = stack->size;
	if (0 == size) {
        mydebug(" empty stack, nothing to print.\n");
		return ;
    }
	for (size_t j = 0; j < size; j++)
		mydebug(" %ld(%d)", stack->nums[stack->idx[j]], stack->part_idx[j]);
	mydebug("|");
}

void print_stack_id(t_stack_ptr stack)
{
    write(1, (const char*)&stack->id, 1);
    write(1, "\n", 1);
}
