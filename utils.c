#include "pushswap.h"

void	print_stacks(t_state *s)
{
	size_t i;

	i = 0;
	ft_printf("a:");
	while (i < s->a_size)
		printf("%ld ", s->a[s->a_idx[i++]]);
	fflush(stdout);
	i = 0;
	ft_printf("\nb:");
	while (i < s->b_size)
		printf("%ld ", s->b[s->b_idx[i++]]);
	fflush(stdout);
}
