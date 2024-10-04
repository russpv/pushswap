#include "pushswap.h"

// create two more arrays that store '1' where a x-tile boundary is
// calculate the median somewhere
// if above median if !flipped push, else rotate then push + flipped
// you are left with a x-tile at top of stack from a prior sort
// rotate the destination stack to correct ascending order
// repeat on that x-tile and push to opposite stack until no more on current stack
// bubble sort swap each x-tile on stack a to finish NO - rotate+swap, might as well push
// insertion sort

// figure out how to manage how many splits to do
// if n>500, n-2n per divide round k, n/(2^k)-n/(2^k)*(n/2) per insertion round * x-tiles
// so for k=2, 4ths, 1500 - >31000 moves
// for k=3, 8ths, 2000 - >16000 moves
// for k=4, 16ths, 2500 - >8000 moves
// for k=5, 32nds, 3000 - >4000 moves



// Recursive for each array group
// We start with a populated stack
// If we are mid-split-round, need to rotate the bottom half back to bottom
// e.g. 44444 | 11111 | 22222 | 33333 >>> 11111 | ... | 44444
void	solve(t_state *s)
{
	int rot_counter = 0;
	size_t i = 0;
	++s->curr_split;
	fprintf(stderr, "currsplit:%d", s->curr_split);
	if (s->curr_split == s->splits)
		return ;
	// TODO target the right range in the right stack
	s->pivot = get_median(s->a, s->a_idx, s->nums);
	while (i < s->nums) /* push and, depending on split, rotate */
	{
		push_b(s); //TODO rewrite moves as function ptr functions
		print_stack(s->a, s->a_idx, s->a_size);
		if (s->a[s->a_idx[i]] < s->pivot) //subsequent splits have bottom at top
		{
			fprintf(stderr, "%ld below|", s->a[s->a_idx[i]]);
			if (1 != s->curr_split)
				rot_b(s); // place value at bottom
			else
				rot_counter++; /* move bottoms later */
		}
		else 
		{
			if (1 != s->curr_split)
			{
				rot_a(s);
				rot_counter++;
			}
		}
		i++;
	}
	while (rot_counter--) //TODO init rot_counter
		rot_b(s); /* move bottoms */
	//solve(s);
}

void	solver(t_state *s)
{
//TODO get a measure of unsortedness, normalized inversion pairs.
	if (s->nums < 200)
		s->splits = 2; //TODO set this to '2'
	else if (s->nums < 600)
		s->splits = 4;
	solve(s);
	return ;
}

