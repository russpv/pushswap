#include "pushswap.h"

bool	is_done(t_state *s)
{
	if (!s)
		err("ERR State not init'd", s);
	if (!s->stacks[0])
		err("ERR Stacks not init'd", s);
	if (is_sorted_asc(s->stacks[0]))
		return (true);
	return (false);
}
