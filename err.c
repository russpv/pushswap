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
	if (LOGGING == true)
	{
		fprintf(stderr, "%s", LTGREY);
		va_list	args;
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		fprintf(stderr, "%s", RESET);
	}	
}

