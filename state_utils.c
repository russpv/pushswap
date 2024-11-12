#include "pushswap.h"

void	print_stacks(t_state *s)
{
	if (true == LOGGING)
	{
		mylog("a:");
		print_stack(s->stacks[STACK_A]);
		mylog("\nb:");
		print_stack(s->stacks[STACK_B]);
		mylog("\n");
	}
}

void	print_stack_name(t_stack_ptr s)
{
	if (get_stack_id(s) == STACK_A)
		write(1, "a\n", 2);
	else
		write(1, "b\n", 2);
}

/* Note: to compile on __APPLE__ use:
 * #if defined(__APPLE__)
 *		ft_vprintf(format, &args);
 * #else
 *		ft_vprintf(format, (va_list *)args);
 * #endif
 */
void	mylog(const char *format, ...)
{
	int		original_stdout;
	va_list	args;

	if (true == LOGGING)
	{
		original_stdout = dup(STDOUT_FILENO);
		if (-1 == original_stdout)
			return ;
		if (-1 == dup2(STDERR_FILENO, STDOUT_FILENO))
		{
			close(original_stdout);
			return ;
		}
		ft_printf("%s", LTGREY);
		va_start(args, format);
		ft_vprintf(format, (va_list *)args);
		va_end(args);
		ft_printf("%s", RESET);
		fflush(stderr);
		if (-1 == dup2(original_stdout, STDOUT_FILENO))
			;
		close(original_stdout);
	}
}
