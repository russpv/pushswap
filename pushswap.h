#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include "stack.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <limits.h>

# define STACKS 2

enum e_stacks
{
	STACK_A,
	STACK_B
};

typedef struct s_state
{
	t_stack_ptr	stacks[STACKS];
	t_stack_ptr	curr_stack;
	t_stack_ptr	dest_stack;

	char	**argv; /* args */
	size_t	nums;

	long		pivot;
	int		passes;	/* algo */
	int		curr_pass;
	int		print_move; /* output */
}	t_state;

// solver.c
void	solver(t_state *);

// state.c
void	init_stack_a(t_state *);
void	create_destination_partitions(t_state *, t_partition_ptr (*)[2]);
// free_struct.c
void	free_struct(t_state *); //TODO rename

// sorted.c
int		sorted(t_state *);

// err.c
void		err(char *, t_state *);

// utils.c
void	print_stacks(t_state *);

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
