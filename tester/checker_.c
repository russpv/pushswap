#include "checker.h"

/* Read-only global variables */
static t_push_move		g_push[MOVE_CNT];
static t_single_move	g_single_moves[MOVE_CNT];
static t_dbl_move		g_double_moves[MOVE_CNT];
static struct s_pair	g_dict[] = {
{"pa", PUSH_A},
{"pb", PUSH_B},
{"ra", ROTATE_A},
{"rb", ROTATE_B},
{"rr", ROTATE_ALL},
{"rra", REV_ROT_A},
{"rrb", REV_ROT_B},
{"rrr", REV_ROT_ALL},
{"sa", SWAP_A},
{"sb", SWAP_B},
{"ss", SWAP_ALL},
};

static inline void	_init_moves(t_state *s)
{
	g_push[PUSH_A] = (t_push_move){push_stack, s->stacks[STACK_A], \
	pop_stack, s->stacks[STACK_B], get_top_partition};
	g_push[PUSH_B] = (t_push_move){push_stack, s->stacks[STACK_B], \
	pop_stack, s->stacks[STACK_A], get_top_partition};
	g_single_moves[SWAP_A] = (t_single_move){swap_stack, \
	s->stacks[STACK_A]};
	g_single_moves[SWAP_B] = (t_single_move){swap_stack, \
	s->stacks[STACK_B]};
	g_single_moves[ROTATE_A] = (t_single_move){rotate_stack, \
	s->stacks[STACK_A]};
	g_single_moves[ROTATE_B] = (t_single_move){rotate_stack, \
	s->stacks[STACK_B]};
	g_single_moves[REV_ROT_A] = (t_single_move){rev_rotate_stack, \
	s->stacks[STACK_A]};
	g_single_moves[REV_ROT_B] = (t_single_move){rev_rotate_stack, \
	s->stacks[STACK_B]};
	g_double_moves[SWAP_ALL] = (t_dbl_move){swap_both};
	g_double_moves[ROTATE_ALL] = (t_dbl_move){rotate_both};
	g_double_moves[REV_ROT_ALL] = (t_dbl_move){rev_rotate_both};
}

// binary search of lexicographically ordered k-v pairs
static enum e_move	_dict_search(char *input)
{
	int	low;
	int	mid;
	int	high;
	int	result;

	low = 0;
	high = MOVE_CNT - 1;
	while (low <= high)
	{
		mid = low + (high - low) / 2;
		result = ft_strncmp(input, g_dict[mid].k, 3);
		if (0 == result)
			return (g_dict[mid].v);
		if (result > 0)
			low = mid + 1;
		else
			high = mid - 1;
	}
	return (NOT_FOUND);
}

// execute move with jumptable
static bool	_do_move(t_state *s, enum e_move move)
{
	if (NOT_FOUND == move)
		return (false);
	if (SWAP_ALL == move || ROTATE_ALL == move || REV_ROT_ALL == move)
		g_double_moves[move].m(s, PRINT_OFF);
	else if (PUSH_A == move || PUSH_B == move)
	{
		if (0 == get_partition_count(g_push[move].st))
			g_push[move].m(g_push[move].st, g_push[move].pop(g_push[move].st2), \
				create_partition(g_push[move].st));
		else if (MAX_PARTITIONS == get_partition_count(g_push[move].st))
			return (false);
		else
			g_push[move].m(g_push[move].st, g_push[move].pop(g_push[move].st2), \
				g_push[move].p(g_push[move].st));
	}
	else
		g_single_moves[move].m(g_single_moves[move].st);
	print_stacks(s);
	return (true);
}

/* get from stdin and send move to do_move()
 * Note: my preference is for descriptive errors
 * but rubric prevents this
 */
static bool	_listen_moves(t_state *s)
{
	char		*line;
	enum e_move	move;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);
		if (NULL == line)
			break ;
		move = _dict_search(line);
		mylog("Got move:%d\n", move);
		if (NOT_FOUND == move)
			err("Error", s);
		if (false == _do_move(s, move))
			err("Error", s);
		free (line);
	}
	if (true == is_sorted_asc(s->stacks[STACK_A]) && \
		true == is_full(s->stacks[STACK_A]))
		ft_putstr_fd("\033[1;32mOK\033[0m\n", STDOUT_FILENO);
	else
		ft_putstr_fd("\033[1;31mKO\033[0m\n", STDOUT_FILENO);
	return (true);
}

/* Note: I prefer more verbose output put rubric prevents it */
int	main(int argc, char **argv)
{
	t_state	*state;
	char	**args;

	args = NULL;
	state = NULL;
	if (argc < 2)
	{
		mylog("Numeric arguments required.\n");
		return (EXIT_FAILURE);
	}
	if (parse_args(&argc, argv, &args) == FAILURE)
		err("Error", state);
	mylog("*** Welcome to checker ***\n");
	create_state(&state, args, argc);
	fill_stack_a(state);
	print_stacks(state);
	_init_moves(state);
	if (false == _listen_moves(state))
		err("Error", state);
	print_stacks(state);
	destroy_state(state);
	ft_freearr(args);
	return (EXIT_SUCCESS);
}
