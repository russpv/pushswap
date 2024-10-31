#include "stack_int.h"

void	mydebug(const char *format, ...)
{
	if (LOGGING == true)
	{
		fprintf(stderr, "%s", GREY);
		va_list	args;
		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
		fprintf(stderr, "%s", RESET); 
	}
}
// TODO delete
void	print_stack(t_stack_ptr stack)
{
	const size_t size = stack->size;
	if (0 == size) {
        mydebug(" empty stack, nothing to print.\n");
		return ;
    }
	for (size_t j = 0; j < size; j++)
		mydebug(" %ld(%d)", stack->nums[stack->idx[j]], stack->part_idx[j]);
	mydebug("|");
}

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
    mydebug("Stack copied (size_%zu, max_%zu, parts_%zu):\n", get_stack_size(s), get_stack_max_size(s),\
            get_partition_count(s));
    print_stack(s);
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
            mydebug("Destroy: part_%d destroyed..\n", stack->partitions[i]->id);
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

/* Returns top value */
long	peek_stack(t_stack_ptr stack)
{
    if (get_stack_size(stack) == 0)
	{
        return (LONG_MAX);
	}
	return (stack->nums[stack->idx[0]]);
}
long	peek_bottom(t_stack_ptr stack)
{
	if (get_stack_size(stack) == 0)
	{
		return (LONG_MAX);
	}
	return (stack->nums[stack->idx[get_stack_size(stack) - 1]]);
}

long    peek_next_stack(t_stack_ptr stack)
{
    if (get_stack_size(stack) < 2)
	{
        return (LONG_MAX);
	}
    return (stack->nums[stack->idx[1]]);
}

/* Removes top value from num array, idx array, and partition 
 * Decrements partition and size */
long	pop_stack(t_stack_ptr stack)
{
    if (NULL == stack || 0 == stack->size)
		return (LONG_MAX);
	const long              top_value = stack->nums[stack->idx[0]];
    const t_partition_ptr   top_partition = get_top_partition(stack);
	stack->nums[stack->idx[0]] = LONG_MAX;
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int)); 
	ft_memmove(&stack->part_idx[0], &stack->part_idx[1], (stack->size - 1) * sizeof(int)); 
	stack->idx[(int)stack->size - 1] = INIT_IDX_VALUE;
	stack->part_idx[(int)stack->size - 1] = INIT_IDX_VALUE;
    if (false == decrement_partition(stack, top_partition->id))
		return (LONG_MAX);
	stack->size--;
    return (top_value);
}

static int	_get_next_free_idx(t_stack_ptr stack)
{
	int i;

	i = -1;
	while (++i < (int)stack->max_size)
		if (stack->idx[i] == INIT_IDX_VALUE)
			return (i);
	if ((int)stack->max_size == i)
		i = -1;
	return (i);
}

/* Adds num to top of stack assuming stack not full and partition has been created */
bool	push_stack(t_stack_ptr stack, int num, t_partition_ptr partition)
{
	const int free_idx = _get_next_free_idx(stack);
	
    if (NULL == partition || -1 == free_idx)
        return (false);
//    mydebug("Log: Pushing %d on s:%d(%d) p:%d\n", num, stack->id, free_idx, \
//			partition->id);
	stack->nums[free_idx] = num;
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->max_size - 1) * sizeof(int));
	stack->idx[0] = free_idx;
	ft_memmove(&stack->part_idx[1], &stack->part_idx[0], (stack->max_size - 1) * sizeof(int));
	stack->part_idx[0] = partition->id;
	stack->size++;
    if (false == increment_partition(stack, partition->id))
		return (false);
    return (true);
}

/* Moves the top element to the last */
bool    rotate_stack(t_stack_ptr stack)
{
	int top_num_idx;
    int top_part_id;
//    const long num = peek_stack(stack);

	if (!stack || !stack->idx)
		return (false);
	if (stack->size <= 1)
		return (true);
	top_num_idx = stack->idx[0];
	ft_memmove(&stack->idx[0], &stack->idx[1], (stack->size - 1) * sizeof(int));
   	stack->idx[stack->size - 1] = top_num_idx;
    top_part_id = stack->part_idx[0];
    ft_memmove(&stack->part_idx[0], &stack->part_idx[1], (stack->size - 1) * sizeof(int));
    stack->part_idx[stack->size - 1] = top_part_id;
//    mydebug("Log: Rotating %d on s:%d(%d) p:%d\n", num, stack->id, top_num_idx, top_part_id);
    return (true);
}

/* Moves the last element to the top */
bool    rev_rotate_stack(t_stack_ptr stack)
{
	int last_num_idx;
    int last_part_id;
//    const long num = peek_bottom(stack); //TODO: write peek_bottom()

	if (!stack || !stack->idx)
        return (false);
	if (stack->size <= 1)
		return (false);
	last_num_idx = stack->idx[stack->size - 1];
	ft_memmove(&stack->idx[1], &stack->idx[0], (stack->size - 1) * sizeof(int));
   	stack->idx[0] = last_num_idx;
    last_part_id = stack->part_idx[stack->size - 1];
    ft_memmove(&stack->part_idx[1], &stack->part_idx[0], (stack->size - 1) * sizeof(int));
    stack->part_idx[0] = last_part_id;
//    mydebug("Log: Reversing %d on s:%d(%d) p:%d\n", num, stack->id, last_num_idx, last_part_id);
    return (true);
}

bool    swap_stack(t_stack_ptr stack)
{
	int second_num_idx;
    
    if (!stack || !stack->idx)
	{
        return (false);
	}
	if (stack->size <= 1)
	{
		return (false);
	}
    second_num_idx = stack->idx[1];
    stack->idx[1] = stack->idx[0];
    stack->idx[0] = second_num_idx;
    return (true);
}

bool    is_sorted_asc(t_stack_ptr s)
{
	size_t i;
	const int *idx = s->idx;
    const long    *nums = s->nums; 

	i = 1;
	if (s->size < (size_t)2) {//mydebug("is sorted -- done since size:%zu", s->size);
		return (true);}
	while (i < s->size)
	{
		if (nums[idx[i - 1]] > nums[idx[i]])
			return (false);
		i++;
	}
    mydebug("is_sorted_asc: sorted!\n");
	return (true);
}

bool    is_sorted_asc_rotationally(t_stack_ptr s)
{
    if (!s)
        return (false);
    if (get_stack_size(s) == 1)
        return (true);
    else if (get_stack_size(s) == 2)
        return (is_sorted_asc(s));
    else if (is_sorted_asc(s) == true)
        return (true);
    size_t i = 0;
    const size_t size = get_stack_size(s);
//    mydebug("is sorted rotationally -- starting loop...(size:%zu)\n", size);
    while (++i < size)
        if (!(get_stack_num(s, i - 1) < get_stack_num(s, i)))
            break;
    //const long max = get_stack_num(s, i - 1);
//    mydebug("is sorted rotationally -- done first loop idx:%zu\n", i);
    while (++i < size)
        if (!(get_stack_num(s, i - 1) < get_stack_num(s, i)))
            break;
    if (i == size) {
        if (peek_bottom(s) < peek_stack(s)) // must be continuous
        {
            mydebug("is sorted rotationally -- it's sorted!!\n");
            return (true); 
        }
    }
    mydebug("is sorted rotationally -- NOT SORTED\n");
    return (false);
}

/* Returns number of rotates or reverse rotates */
int    get_moves_back_to_head(t_stack_ptr s)
{
    const size_t size = get_stack_size(s);
    int i = 0;

    if (size == 1)
        return (0);
    if (is_sorted_asc(s)) {mydebug("moves back: is_sorted says sorted\n");
        return (0); }
    if (size == 2)
        return (1);
    while (++i < (int)size)
        if (!(get_stack_num(s, i - 1) < get_stack_num(s, i)))
            break ;
    if (i < get_middle_idx(s)) /* is the top idx */ 
        return (i);
    else
        return (-1 * ((int)size - i)); /* inclusive */

    
}

void print_stack_id(t_stack_ptr stack)
{
    write(1, (const char*)&stack->id, 1);
    write(1, "\n", 1);
}

/* Returns true if both are the same */
bool	stack_compare(int a, t_stack_ptr b)
{
	if (a == b->id)
		return (true);
	return (false);
}

int		get_stack_id(t_stack_ptr s)
{
	if (!s)
		return (-1);
	return(s->id);
}

size_t	get_stack_size(t_stack_ptr s)
{
	if (!s)
		return (SIZE_MAX);
	return (s->size);
}
bool	is_full(t_stack_ptr s)
{
	if (!s)
		return (false);
	return (s->size == s->max_size);
}

long	get_stack_num(t_stack_ptr s, int idx)
{
	if (!s)
		return (LONG_MAX);
    if (idx >= (int)get_stack_size(s))
        return (LONG_MAX);
	return (s->nums[s->idx[idx]]);
}

/* Returns lower middle index for even size */
int		get_middle_idx(t_stack_ptr s)
{
	return (get_stack_size(s) / 2);
}

size_t   get_stack_max_size(t_stack_ptr s)
{
    if (!s)
        return (SIZE_MAX);
    return (s->max_size);
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

static bool   _update_stack_extremum(t_stack_ptr s)
{
    if (s->size == 0)
        return (false);
    s->min = LONG_MAX;
    s->max = LONG_MIN;
    long testnum;
    int i = -1;

    while (++i < (int)s->size)
    {
        testnum = get_stack_num(s, i);
        if (testnum < s->min)
            s->min = testnum;
        if (testnum > s->max)
            s->max = testnum;
    }
    return (true);
}

/* Tests if num is beyond stack min or max */
bool    is_stack_extremum(t_stack_ptr s, long num)
{
    if (s->last_update_size != s->size)
        if (_update_stack_extremum(s) == false)
            return (false);
    if (num < s->min || num > s->max)
        return (true);
    return (false);
}

/* Accounts for wrap-around */
int get_stack_idx_at_depth(t_stack_ptr s, int depth)
{
    if (s->size < 2)
        return (INT_MAX);
    while (depth < 0)
        depth += s->size;
    return (depth % s->size);
}

