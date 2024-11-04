#include "pushswap.h"
#include <stdarg.h>

void	err(char *msg, t_state *state)
{
	destroy_state(state);
	ft_printf(msg);
	exit(EXIT_FAILURE);
}

void	mylog(const char *format, ...)
{
	int	original_stdout;
		va_list args;

	if (LOGGING == true)
	{
		original_stdout = dup(STDOUT_FILENO);
		dup2(STDERR_FILENO, STDOUT_FILENO);
		ft_printf("%s", LTGREY);
		va_start(args, format);
		ft_printf(format, args);
		va_end(args);
		ft_printf("%s", RESET);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}
