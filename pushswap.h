#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include "stack.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define STACKS 2
# define RED "\033[31m"
# define GREY "\033[90m"
# define LTGREY "\033[38;5;245m"
# define RESET "\033[0m"
# define PRINT_ON 1
# define PRINT_OFF 0
# define LOGGING true
# define TOP_IDX 0

enum			e_stacks
{
	STACK_A,
	STACK_B
};

enum			e_move_type
{
	PUSH,
	ROTATE,
	REV_ROTATE,
	SWAP,
	MOVE_COUNT
};

typedef void	(*t_move)(t_stack_ptr, va_list);

/* State data type */

typedef struct s_state
{
	t_stack_ptr	stacks[STACKS];
	t_stack_ptr	curr_stack;
	t_stack_ptr	dest_stack;

	char **argv; /* args */
	size_t		nums;

	long		pivot;
	int passes; /* algo */
	int			curr_pass;
	int print_move; /* output */

	int			target_depth;
	int			moves;
	int			depth;

	int			tmp;
	int			tmp_moves;
	int			tmp_depth;
	int			tmp_t_depth;
}				t_state;

// state.c
void			create_state(t_state **, char **, int);
void			destroy_state(t_state *);
bool	fill_stack_a(t_state *); // DELETE
void			create_destination_partitions(t_state *,
					t_partition_ptr (*)[2]);
void			flip_curr_stack(t_state *);
bool			is_done(t_state *);

void			move(t_stack_ptr stack, enum e_move_type, ...);
void			swap_both(t_state *);
void			rotate_both(t_state *);
void			rev_rotate_both(t_state *);

/* Program functions */
// greedy_sort.c
bool			greedy_sort(t_state *s);

// solver.c
void			solver(t_state *);
void			solve(t_state *);
// sorted.c
int				sorted(t_state *);

// err.c
void			err(char *, t_state *);
void			mylog(const char *format, ...);

// utils.c
void			print_stacks(t_state *);

#endif
