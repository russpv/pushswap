

void	rev_rot_both(t_state *s)
{
	int tmp;

	tmp = s->print_move;
	s->print_move = 0;
	rev_rot_a(s);
	rev_rot_b(s);
	s->print_move = tmp;
	write(1, "rrr\n", 4);
}

void	rev_rot_b(t_state *s)
{
	int last_num;

	if (!s->b || !s->b_idx)
		err("illegal operation", s);
	if (s->b_size <= 1)
		return ;
	last_num = s->b_idx[s->b_size - 1];
	ft_memmove(&s->b_idx[1], &s->b_idx[0], (s->b_size - 1) * sizeof(int));
	s->b_idx[0] = last_num;
	if (s->print_move)
		write(1, "rrb\n", 3);
}

/* bottom number becomes top number */
void	rev_rot_a(t_state *s)
{
	int last_num;

	if (!s->a || !s->a_idx)
		err("illegal operation", s);
	if (s->a_size <= 1)
		return ;
	last_num = s->a_idx[s->a_size - 1];
	ft_memmove(&s->a_idx[1], &s->a_idx[0], (s->a_size - 1) * sizeof(int));
	s->a_idx[0] = last_num;
	if (s->print_move)
		write(1, "rra\n", 3);
}
