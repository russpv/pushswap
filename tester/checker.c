#include "checker.h"

# define BUFFER_SIZE 10

/* Read-only global variables */
static t_push_move push[MOVE_CNT];
static t_single_move single_moves[MOVE_CNT];
static t_dbl_move double_moves[MOVE_CNT];
static struct pair dict[] = {
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

// binary search of lexicographically ordered k-v pairs
static enum e_move	_dict_search(char *input)
{
	int low;
	int mid;
	int high;
	int result;

	low = 0;
	high = MOVE_CNT - 1;
	while (low <= high)
	{
		mid = low + (high - low) / 2;
		result = ft_strncmp(input, dict[mid].k, 3);
		if (0 == result)
			return (dict[mid].v);
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
		double_moves[move].m(s, PRINT_OFF);
	else if (PUSH_A == move || PUSH_B == move)
	{
		if (MAX_PARTITIONS == get_partition_count(push[move].st))
			return (false);
		else
			push[move].m(\
				push[move].st, \
				push[move].pop(push[move].st2), \
				push[move].p(push[move].st));
	}
	else
		single_moves[move].m(single_moves[move].st);
	print_stacks(s);
	return (true);
}

static char *_get_next_line(int fd)
{
	char c;
	char buf[BUFFER_SIZE];
	int	bytes;
	unsigned long i;

	i = 0;
	c = 0;
	while (i < sizeof(buf) - 2)
	{
		bytes = read(fd, &c, 1);
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
		line = _get_next_line(STDIN_FILENO);
		if (NULL == line)
			break ;
		move = _dict_search(line);
		mylog("Got move:%d\n", move);
		if (NOT_FOUND == move)
			err("Error: bad move", s);
		if (false == _do_move(s, move))
			err("Error: couldn't do move", s);
		free (line);
	}
	if (false == is_sorted_asc(s->stacks[STACK_A]))
		ft_putstr_fd("\033[1;31mK.O\n", STDOUT_FILENO);
	else
		ft_putstr_fd("\033[1;32mO.K\n", STDOUT_FILENO);
	return (true);
}

/* Note: I prefer more verbose output put rubric prevents it */
int main (int argc, char **argv)
{
	t_state	*state;
	char **args;
	
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
