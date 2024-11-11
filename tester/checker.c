#include "checker.h"

# define BUFFER_SIZE 4

/* Read-only global variables */
static t_push_move push[MOVE_CNT];
static t_single_move single_moves[MOVE_CNT];
static t_dbl_move double_moves[MOVE_CNT];
static struct pair dict[] = {
	{"pa", PUSH_A},
	{"pb", PUSH_B},
	{"sa", SWAP_A},
	{"sb", SWAP_B},
	{"ss", SWAP_ALL},
	{"ra", ROTATE_A},
	{"rb", ROTATE_B},
	{"rr", ROTATE_ALL},
	{"rra", REV_ROT_A},
	{"rrab", REV_ROT_B},
	{"rrr", REV_ROT_ALL}
};

static inline void _init_moves(t_state *s) {
    push[PUSH_A] = (t_push_move){push_stack, s->stacks[STACK_A], pop_stack, s->stacks[STACK_B], get_top_partition};
    push[PUSH_B] = (t_push_move){push_stack, s->stacks[STACK_B], pop_stack, s->stacks[STACK_A], get_top_partition};

    single_moves[SWAP_A] = (t_single_move){swap_stack, s->stacks[STACK_A]};
    single_moves[SWAP_B] = (t_single_move){swap_stack, s->stacks[STACK_B]};
    single_moves[ROTATE_A] = (t_single_move){rotate_stack, s->stacks[STACK_A]};
    single_moves[ROTATE_B] = (t_single_move){rotate_stack, s->stacks[STACK_B]};
    single_moves[REV_ROT_A] = (t_single_move){rev_rotate_stack, s->stacks[STACK_A]};
    single_moves[REV_ROT_B] = (t_single_move){rev_rotate_stack, s->stacks[STACK_B]};

    double_moves[SWAP_ALL] = (t_dbl_move){swap_both};
    double_moves[ROTATE_ALL] = (t_dbl_move){rotate_both};
    double_moves[REV_ROT_ALL] = (t_dbl_move){rev_rotate_both};
}

/* binary search of lexicographically ordered k-v pairs */
static enum e_move	_dict_search(char *input)
{
	int low;
	int mid;
	int high;
	int result;

	low = 0;
	high = MOVE_CNT - 1;
	while (low < high)
	{
		mid = low + (high - low) / 2;
		result = ft_strncmp(input, dict[mid].k, 3);
		if (0 == result)
			return (dict[mid].v);
		if (result > 0) // pos means right, neg means left
			low = mid + 1;
		else
			high = mid - 1;
	}
	return (NOT_FOUND);
}

// execute move with jumptable
static bool	_do_move(t_state *s, enum e_move move)
{
	ft_printf("In do_move\n");	
	if (NOT_FOUND == move)
		return (false);
	if (SWAP_ALL == move || ROTATE_ALL == move || REV_ROT_ALL == move)
		double_moves[move].m(s);
	else if (PUSH_A == move || PUSH_B == move)
		push[move].m(push[move].st, push[move].pop(push[move].st2), \
		push[move].p(push[move].st2));
	else
		single_moves[move].m(single_moves[move].st);
	return (true);
}

char *get_next_line(int fd)
{
	char c;
	char buf[BUFFER_SIZE];
	int	bytes;
	unsigned long i;

	i = 0;
	c = 0;
	while (i < sizeof(buf) - 1)
	{
		bytes = read(fd, &c, sizeof(buf));
		if (bytes < 0)
			return (NULL);
		if (0 == bytes || '\n' == c)
			break ;
		buf[i++] = c;
	}
	if (0 == i)
		return (NULL);
	buf[i] = '\0';
	return (ft_strdup(buf));
}

// get from stdin and send move to do_move()
static bool	_listen_moves(t_state *s)
{
	char *line;
	enum e_move move;

	while (true)
	{
		line = get_next_line(STDIN_FILENO);;
		if (NULL == line)
			break ;
		move = _dict_search(line);
		if (NOT_FOUND == move)
			err("Error: bad move", s);
		_do_move(s, move);
		free (line);
	}
	if (false == is_sorted_asc(s->stacks[STACK_A]))
		ft_putstr_fd("\033[1;31mK.O\n", 1);
	else
		ft_putstr_fd("\033[1;32mO.K\n", 1);
	return (true);
}

int main (int argc, char **argv)
{
	t_state	*state;
	char **args;

	args = NULL;
	state = NULL;
	if (argc < 2)
		err("Requires numeric arguments to check", state);
	if (parse_args(&argc, argv, &args) == FAILURE)
		err("Error", state);
	create_state(&state, argv, argc);
	_init_moves(state);
	if (false == _listen_moves(state))
		err("Error", state);
	destroy_state(state);
	ft_freearr(args);
	return (EXIT_SUCCESS);
}
