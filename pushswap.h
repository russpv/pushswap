#ifndef PUSHSWAP_H
# define PUSHSWAP_H

# include "hashtable.h"
# include "libft/libft.h"
# include "stack.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define STACKS 2
# define MAX_NUMS 5000
# define RED "\033[31m"
# define GREY "\033[90m"
# define LTGREY "\033[38;5;245m"
# define RESET "\033[0m"
# define PRINT_ON 1
# define PRINT_OFF 0
# define TOP_IDX 0

# ifdef CHECKER_MODE
#  define DEBUGGING 1
#  define LOGGING 1
# else
#  define DEBUGGING 0
#  define LOGGING 0
# endif

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

typedef void	(*t_move)(t_stack_ptr a, va_list b);

/* State data type */

typedef struct s_state
{
	t_stack_ptr	stacks[STACKS];
	t_stack_ptr	curr_stack;
	t_stack_ptr	dest_stack;

	char		**args;
	size_t		nums;

	long		pivot;
	int			passes;
	int			curr_pass;
	int			print_move;

	int			target_depth;
	int			moves;
	int			depth;

	long		tmp;
	int			tmp_i;
	int			tmp_moves;
	int			tmp_depth;
	int			tmp_t_depth;
}				t_state;

// main.c
int				parse_args(int *a, char **b, char ***c);

// state.c
void			create_state(t_state **a, char **b, int c);
void			destroy_state(t_state *a);
bool			fill_stack_a(t_state *a);
void			create_destination_partitions(t_state *a,
					t_partition_ptr (*b)[2]);
void			flip_curr_stack(t_state *a);
bool			is_done(t_state *a);

void			move(t_stack_ptr a, enum e_move_type b, ...);

void			swap_both(t_state *a, int b);
void			rotate_both(t_state *a, int b);
void			rev_rotate_both(t_state *a, int b);

void			print_stacks(t_state *a);
void			print_stack_name(t_stack_ptr a);

/* Program functions */

// greedy_sort.c
bool			greedy_sort(t_state *a);
long			distance(long a, long b);
int				find_alts(t_state *a, int b);
int				get_total_moves(t_state *a, int b);
int				find_cheapest_idx(t_state *a);
int				get_moves(const int a, const int b);

long			myabs(long a);
int				abs_min(int a, int b);
int				abs_max(int a, int b);

// solver.c
void			solver(t_state *a);

// quicksort.c
void			quicksort_partition(t_state *a, t_partition_ptr b,
					t_partition_ptr c, size_t d);

// utils.c
void			err(char *a, t_state *b);
void			printarr(char **a);
void			mylog(const char *a, ...);

// argparse.c argtests.c
int				check_dupes(char **a, size_t b);
int				check_word(char *a);
int				has_digits(char *a);

#endif
