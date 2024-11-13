#include "stack_int.h"

static inline void	_do_init_loop(t_stack_ptr stack)
{
	size_t	i;

	i = -1;
	while (++i < MAX_PARTITIONS)
		stack->partitions[i] = NULL;
	i = -1;
	while (++i < stack->max_size)
	{
		stack->nums[i] = INIT_NUM_VALUE;
		stack->idx[i] = INIT_IDX_VALUE;
		stack->part_idx[i] = INIT_IDX_VALUE;
	}
	return ;
}

t_stack_ptr	create_stack(const char id, const size_t size)
{
	t_stack_ptr	st;

	if (!(size > 0))
		return (mydebug("ERR stack size 0\n"), NULL);
	st = malloc(sizeof(struct s_stack));
	if (st)
	{
		st->id = id;
		st->max_size = size;
		st->size = 0;
		st->partition_count = 0;
		st->nums = malloc(sizeof(long) * size);
		st->idx = malloc(sizeof(int) * size);
		st->part_idx = malloc(sizeof(int) * size);
		st->tmp = 0;
		st->min = 0;
		st->max = 0;
		st->last_update_size = 0;
		if (!st->nums || !st->idx || !st->part_idx)
			return (free(st->nums), free(st->idx), free(st->part_idx), NULL);
		_do_init_loop(st);
	}
	mydebug("---- (created stack) max_size:%zu id:%d\n", st->max_size, st->id);
	return (st);
}

t_stack_ptr	copy_stack(t_stack_ptr src)
{
	t_stack_ptr	s;
	size_t		i;

	s = create_stack(get_stack_id(src), src->max_size);
	if (!s)
		return (NULL);
	s->size = src->size;
	s->min = src->min;
	s->max = src->max;
	s->last_update_size = src->last_update_size;
	i = SIZE_MAX;
	while (++i < s->max_size)
		s->nums[i] = src->nums[i];
	i = SIZE_MAX;
	while (src->idx[++i] != INIT_IDX_VALUE)
		s->idx[i] = src->idx[i];
	i = SIZE_MAX;
	while (src->part_idx[++i] != INIT_IDX_VALUE)
		s->part_idx[i] = src->part_idx[i];
	i = SIZE_MAX;
	while (++i < get_partition_count(src))
		copy_partition(src->partitions[i], s);
	mydebug("---- (copy_stack) -- (size_%zu, max_%zu, parts_%zu): ",
		get_stack_size(s), get_stack_max_size(s), get_partition_count(s));
	return (s);
}

void	destroy_stack(t_stack_ptr stack)
{
	int	i;

	i = -1;
	while (++i < MAX_PARTITIONS)
	{
		if (stack->partitions[i] != NULL)
		{
			mydebug("---- (destroy_part): part_%d destroyed..\n",
				stack->partitions[i]->id);
			destroy_partition(&stack->partitions[i]);
		}
	}
	free(stack->idx);
	free(stack->part_idx);
	free(stack->nums);
	free(stack);
}

/* Populates nums and idx only on empty stack
 * Expects an argument array of C strings */
bool	fill_stack(t_stack_ptr stack, char **args)
{
	size_t	i;

	i = 0;
	if (NULL == stack)
		return (false);
	while (args[i] != NULL)
	{
		stack->nums[i] = (long)ft_atoi(args[i]);
		stack->idx[i] = i;
		stack->size++;
		i++;
	}
	return (true);
}

/*
static long	_myabs(long num)
{
	if (num < 0)
		return (-num);
	else
		return (num);
}

// Entropy measures sorted-ness; captures degree not rotated to ascending order
// Accepts optional partition to compute only for that partition
unsigned long	get_entropy(t_stack_ptr s, t_partition_ptr p)
{
	int				i;
	int				j;
	unsigned long	sum;

	if (s->size < 2)
		return (0);
	if (p != NULL)
		if (p->size < 2)
			return (0);
	i = 0;
	j = 0;
	sum = 0;
	if (p != NULL)
	{
		while (++j < (int)p->size)
		{
			while (s->part_idx[i] != p->id)
				i++;
			sum += _myabs(s->nums[s->idx[i]] - s->nums[s->idx[i - 1]]);
		}
	}
	else
	{
		while (++i < (int)s->size)
			sum += _myabs(s->nums[s->idx[i]] - s->nums[s->idx[i - 1]]);
	}
	return (sum);
}
*/
