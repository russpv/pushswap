
/* These are swap moves */
/* top numbers switch places */
void	swap_both(t_state *s)
{
	int tmp;

	tmp = s->print_move;
	s->print_move = 0;
	swap_a(s);
	swap_b(s);
	s->print_move = tmp;
	write(1, "ss\n", 3);
}

void	swap_b(t_state *s)
{
	size_t tmp;

	if (!s->b || !s->b_idx)
		err("illegal operation", s);
	if (s->b_size <= 1)
		return ;
	tmp = s->b_idx[0];
	s->b_idx[0] = s->b_idx[1];
	s->b_idx[1] = tmp;
	if (s->print_move)
		write(1, "sb\n", 3);
}

void	swap_a(t_state *s)
{
	size_t tmp;

	if (!s->a || !s->a_idx)
		err("illegal operation", s);
	if (s->a_size <= 1)
		return ;
	tmp = s->a_idx[0];
	s->a_idx[0] = s->a_idx[1];
	s->a_idx[1] = tmp;
	if (s->print_move)
		write(1, "sa\n", 3);
}
