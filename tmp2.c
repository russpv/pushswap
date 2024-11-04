
// no matter the state, the current partition will be absolutely larger or
// smaller than whats on the dest stack.
// B>A will be smaller
// A>B will be larger
// I want this to run only B>A, the final pass.
// can write this per partition or per stack. might as well per stack.
/* This is run only on Stack B */
bool	insertion_sort(t_state *s)
{
	int i = 0;
	const t_stack_ptr b = s->curr_stack;
	const t_stack_ptr a = s->dest_stack;
	const t_partition_ptr partition = create_partition(a);
	while (i < (int)s->nums) /* iterate over all nums */
	{
		size_t rotates = 0;
		size_t rev_rotates = 0;
		int rot_counter = 0;
		int rev_counter = 0;

		if (peek_next_stack(b) > peek_stack(b)) /* swap */
			move(b, SWAP, 0, NULL, PRINT_ON);
		const long num = peek_stack(b);
		if (get_stack_size(a) >= 1)
		{
			fprintf(stderr, "stack size=_%zu\n", get_stack_size(a));
			fflush(stderr);
			if (num > peek_bottom(a)) /* new maximum */
				rotates = 1;
			else if (num < peek_stack(a)) /* new min */
				;
			else
			{
				while (num > get_stack_num(a, rotates)
					&& rotates < get_stack_size(a))
					rotates++;
				while (num < get_stack_num(a, get_stack_size(a) - 1
						- rev_rotates) && rev_rotates < get_stack_size(a))
					rev_rotates++;
			}
		}
		bool do_rotates = false;
		bool do_rev_rotates = false;
		fprintf(stderr, "Counts Rot:%zu Rev:%zu\n", rotates, rev_rotates);
		if (rev_rotates == 0 && rotates == 0) /* new min num */
			;
		else if (rev_rotates > rotates && rotates > 0 && get_stack_size(a) > 1)
		{
			do_rotates = true;
			fprintf(stderr, "Rotating\n");
			while (rotates-- > 0)
			{
				move(a, ROTATE, 0, NULL, PRINT_ON);
				rot_counter++;
			}
		}
		else if (rev_rotates <= rotates && rev_rotates > 0
			&& get_stack_size(a) > 1)
		{
			do_rev_rotates = true;
			fprintf(stderr, "Reverse rotating\n");
			rev_counter = 1; /* have to reverse the just-pushed num */
			while (rev_rotates-- > 0)
			{
				move(a, REV_ROTATE, 0, NULL, PRINT_ON);
				rev_counter++;
			}
		}
		move(a, PUSH, pop_stack(b), partition, PRINT_ON);
		if (rev_rotates == 0 && rotates == 1) /* new max num */
		{
			mylog("new maximum\n");
			move(a, ROTATE, 0, NULL, PRINT_ON);
		}
		// check if the next num should be pushed on the way back
		else if (get_stack_size(a) == 2)
		{
			if (rotates > 0)
			{
				mylog("Rotating\n");
				move(a, ROTATE, 0, NULL, PRINT_ON);
			}
		}
		else if (do_rotates == true)
		{
			while (rot_counter-- > 0)
			{
				move(a, REV_ROTATE, 0, NULL, PRINT_ON);
				if (peek_stack(b) < peek_stack(a)
					&& peek_stack(b) > peek_bottom(a))
					move(a, PUSH, pop_stack(b), partition, 0, NULL, PRINT_ON);
			}
		}
		else if (do_rev_rotates == true)
		{
			while (rev_counter-- > 0)
			{
				move(a, ROTATE, 0, NULL, PRINT_ON);
				if (peek_stack(b) < peek_stack(a)
					&& peek_stack(b) > peek_bottom(a))
					move(a, PUSH, pop_stack(b), partition, PRINT_ON);
			}
		}
		i++;
	}
	print_stacks(s);
	return (true);
}