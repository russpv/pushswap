#include "pushswap.h"

void	rot_both(t_state *s)
{
	int tmp;

	tmp = s->print_move;
	s->print_move = 0;
	rot_a(s);
	rot_b(s);
	s->print_move = tmp;
	write(1, "rr\n", 3);
}

void	rot_b(t_state *s)
{
	int first_num;

	if (!s->b || !s->b_idx)
		err("illegal operation", s);
	if (s->b_size <= 1)
		return ;
	first_num = s->b_idx[0];
	ft_memmove(&s->b_idx[0], &s->b_idx[1], (s->b_size - 1) * sizeof(int));
	s->b_idx[s->b_size - 1] = first_num;
	if (s->print_move)
		write(1, "rb\n", 3);
}

/* bottom number becomes top number */
void	rot_a(t_state *s)
{
	int first_num;

	if (!s->a || !s->a_idx)
		err("illegal operation", s);
	if (s->a_size <= 1)
		return ;
	first_num = s->a_idx[0];
	ft_memmove(&s->a_idx[0], &s->a_idx[1], (s->a_size - 1) * sizeof(int));
	s->a_idx[s->a_size - 1] = first_num;
	if (s->print_move)
		write(1, "ra\n", 3);
}
