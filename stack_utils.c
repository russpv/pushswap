#include "stack_int.h"

void	mydebug(const char *format, ...)
{
	int		original_stdout;
	va_list	args;

	if (true == DEBUGGING)
	{
		original_stdout = dup(STDOUT_FILENO);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		ft_printf("%s", GREY);
		va_start(args, format);
		#if defined(__APPLE__)
		ft_vprintf(format, &args);
		#else
		ft_vprintf(format, (va_list*)args);
		#endif
		va_end(args);
		ft_printf("%s", RESET);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}

void	print_stack(t_stack_ptr stack)
{
	const size_t	size = stack->size;
	size_t			i;

	if (0 == size)
	{
		mydebug(" empty stack, nothing to print.\n");
		return ;
	}
	i = -1;
	while (++i < size)
		mydebug(" %ld(%d)", stack->nums[stack->idx[i]], stack->part_idx[i]);
	mydebug("|\n");
}

void	print_stack_id(t_stack_ptr stack)
{
	write(1, (const char *)&stack->id, 1);
	write(1, "\n", 1);
}
