
/* Moves top number on a to stack b */
void	push_b(t_state *s)
{
	int num;

	if (!s->a || !s->a_idx || !s->b || !s->b_idx)
		err("illegal operation", s);
	if (0 == s->a_size)
		return ;
	// pop from stack a
	// TODO include parts array
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
		write(1, "pb\n", 3);
}

/* Moves top number on b to stack a */
void	push_a(t_state *s)
{
	int num;

	if (!s->a || !s->a_idx || !s->b || !s->b_idx)
		err("illegal operation", s);
	if (0 == s->b_size)
		return ;
	// pop from stack b
	num = s->b[s->b_idx[0]];
	s->b[s->b_idx[0]] = LONG_MAX;
	ft_memmove(&s->b_idx[0], &s->b_idx[1], (s->b_size - 1) * sizeof(size_t));
	s->b_size--;
	// push to stack a
	ft_memmove(&s->a_idx[1], &s->a_idx[0], (s->a_size) * sizeof(size_t));
	s->a[s->a_size] = num; // put new numaer at the end
	s->a_idx[0] = s->a_size; // point to it from zeroth position
	s->a_size++;
	if (s->print_move)
		write(1, "pa\n", 3);
}
