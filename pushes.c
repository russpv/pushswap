#include "pushswap.h"

/* Moves top number to opposite stack */
void	push_a(t_state *s)
{
	int num;

	if (!s->a || !s->a_idx || !s->b || !s->b_idx)
		err("illegal operation", s);
	if (0 == s->a_size)
		return ;
	// pop from stack a
	num = s->a[s->a_idx[0]];
	s->a[s->a_idx[0]] = LONG_MAX;
	ft_memmove(&s->a_idx[0], &s->a_idx[1], (s->a_size - 1) * sizeof(size_t));
	s->a_size--;
	// push to stack b
	ft_memmove(&s->b_idx[1], &s->b_idx[0], (s->b_size) * sizeof(size_t));
	s->b[s->b_size] = num; // put new number at the end
	s->b_idx[0] = s->b_size; // point to it from zeroth position
	s->b_size++;
	if (s->print_move)
		write(1, "pa\n", 3);
}
