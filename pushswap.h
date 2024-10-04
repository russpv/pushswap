#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>

/*
typedef struct s_stack
{
	int		*nums; // actual integers, not modified
	int		*idx;  // order index of nums, ops modify this
	//size_t	head; // not necessary, index 0
	size_t	size;
	size_t	last_idx; // for readability
	int		max; // quantity
	int		min;
	int		median_idx;
}	t_stack;
*/
typedef struct s_state
{
	long		*a; /* stacks */
	size_t	*a_idx;
	size_t	a_size;
	long		*b;
	size_t	*b_idx;
	size_t	b_size;

	char	**argv; /* args */
	size_t	nums;

	long		pivot;
	int		splits;	/* algo */
	int		curr_split;
	int		curr_stack;
	int		print_move; /* output */
}	t_state;

// free_struct.c
void	free_struct(t_state *);

// solver.c
void	solver(t_state *);

// init_stack_a.c
void	init_stack_a(t_state *);

// sorted.c
int		sorted(t_state *);

// err.c
void		err(char *, t_state *);

// utils.c
int		get_median(long *, size_t *, size_t);
void	print_stacks(t_state *);
void	print_stack(long *, size_t *, size_t);

// rev_rotates.c rotates.c pushes.c
void	rev_rot_a(t_state *);
void	rev_rot_b(t_state *);
void	rev_rot_both(t_state *);
void	rot_a(t_state *);
void	rot_b(t_state *);
void	rot_both(t_state *);
void	swap_a(t_state *);
void	swap_b(t_state *);
void	swap_both(t_state *);
void	push_a(t_state *);
void	push_b(t_state *);

#endif
