#include "pushswap.h"

void	rotate_both(t_state *s)
{
	move(s->stacks[0], ROTATE, 0, NULL, PRINT_OFF);
	move(s->stacks[1], ROTATE, 0, NULL, PRINT_OFF);
	write(1, "rr\n", 3);
}

void	rev_rotate_both(t_state *s)
{
	move(s->stacks[0], REV_ROTATE, 0, NULL, PRINT_OFF);
	move(s->stacks[1], REV_ROTATE, 0, NULL, PRINT_OFF);
	write(1, "rrr\n", 4);
}

void	swap_both(t_state *s)
{
	move(s->stacks[0], SWAP, 0, NULL, PRINT_OFF);
	move(s->stacks[1], SWAP, 0, NULL, PRINT_OFF);
	write(1, "ss\n", 3);
}
