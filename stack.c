#include "stack_int.h"

t_stack_ptr	create_stack(const char id, const size_t size)
{
	size_t i;

	t_stack_ptr stack = malloc(sizeof(struct s_stack));
	if (stack)
	{
		stack->id = id;
		stack->max_size = size;
        stack->partition_count = 0;
		stack->nums = malloc(sizeof(long) * size);
		stack->idx = malloc(sizeof(int) * size);
        stack->part_idx = malloc(sizeof(int) * size);
        stack->min = 0;
        stack->max = 0;
        stack->last_update_size = 0;
		if (!stack->nums || !stack->idx || !stack->part_idx)
            return (free(stack->nums), free(stack->idx), free(stack->part_idx), NULL);
        i = 0;
        while (i < size)
        {
            if (i < MAX_PARTITIONS)
			    stack->partitions[i] = NULL;
            stack->nums[i] = INIT_NUM_VALUE;
            stack->idx[i] = INIT_IDX_VALUE;
            stack->part_idx[i++] = INIT_IDX_VALUE; 
        }
	}	
	return (stack);
}

t_stack_ptr	copy_stack(t_stack_ptr src)
{
	t_stack_ptr s = create_stack(get_stack_id(src), src->max_size);
	if (!s)
		return (NULL);
    size_t i;

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
    mydebug("---- (copy_stack) -- (size_%zu, max_%zu, parts_%zu): ", get_stack_size(s), get_stack_max_size(s),\
            get_partition_count(s));
	return (s);
}

void	destroy_stack(t_stack_ptr stack)
{
	int i;

	i = -1;
	while (++i < MAX_PARTITIONS)
	{
		if (stack->partitions[i] != NULL)
        {
            mydebug("---- (destroy_part): part_%d destroyed..\n", stack->partitions[i]->id);
			destroy_partition(&stack->partitions[i]);
        }
	}
	free(stack->idx);
    free(stack->part_idx);
	free(stack->nums);
	free(stack);
}

/* Populates nums and idx only */
bool	fill_stack(t_stack_ptr stack, char **argv)
{
    size_t i;

    i = 0;
    if (NULL == stack)
		return (false);
	while (argv[i + 1] != NULL)
    {
        stack->nums[i] = (long)ft_atoi(argv[i + 1]);
        stack->idx[i] = i;
        stack->size++;
        i++;
    }
	return (true);
}





static long _myabs(long num)
{
    if (num < 0)
        return (-num);
    else
        return (num);
}

/* Entropy measures sorted-ness; captures degree not rotated to ascending order */
/* Accepts optional partition to compute only for that partition */
unsigned long get_entropy(t_stack_ptr s, t_partition_ptr p)
{
    if (s->size < 2)
        return (0);
    if (p != NULL)
        if (p->size < 2)
            return (0);
    int             i = 0;
    int             j = 0;
    unsigned long   sum = 0;
    
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






