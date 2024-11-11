#ifndef CHECKER_H
# define CHECKER_H

# include "../pushswap.h"

enum	e_move 
{
	PUSH_A,
	PUSH_B,
	SWAP_A,
	SWAP_B,
	SWAP_ALL,
	ROTATE_A,
	ROTATE_B,
	ROTATE_ALL,
	REV_ROT_A,
	REV_ROT_B,
	REV_ROT_ALL,
	MOVE_CNT,
	NOT_FOUND,
};

struct pair {
	char *k;
	int v;
};

typedef	void	(*t_dmovef)(t_state *s);
typedef bool	(*t_smovef)(t_stack_ptr s);
typedef t_partition_ptr (*t_part)(t_stack_ptr s);
typedef long	(*t_pop)(t_stack_ptr s);
typedef bool 	(*t_push)(t_stack_ptr, int, t_partition_ptr);

typedef struct s_move_push {
	t_push m;
	t_stack_ptr st;
	t_pop pop;
	t_stack_ptr st2;
	t_part p;
} t_push_move;

typedef struct s_move_single {
	t_smovef m;
	t_stack_ptr st;
} t_single_move;

typedef struct s_move_dbl {
	t_dmovef m;
} t_dbl_move;


#endif
