#include "pushswap.h"

long	myabs(long a)
{
	if (a < 0)
		return (-a);
	else
		return (a);
}

/* Returns absolute value minimum and original args */
int	abs_min(int a, int b)
{
	const long	i = myabs((long)a);
	const long	j = myabs((long)b);

	if (i < j)
		return (a);
	else if (j < i)
		return (b);
	else
		return (a);
}

int	abs_max(int a, int b)
{
	const long	i = myabs((long)a);
	const long	j = myabs((long)b);

	if (i < j)
		return (b);
	else if (i > j)
		return (a);
	else
		return (a);
}
