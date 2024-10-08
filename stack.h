#ifndef STACK_H
# define STACK_H

/* Stack ADT */
typedef struct s_stack *t_stack_ptr;

t_stack_ptr create_stack(const char id, const size_t size);
void destroy_stack(t_stack_ptr stack);
void push_stack(t_stack_ptr stack, int num);
void pop_stack(t_stack_ptr stack);
void rotate_stack(t_stack_ptr stack);
void rev_rotate_stack(t_stack_ptr stack);

struct s_partition *create_partition(t_stack_ptr stack);
void destroy_partition(struct s_partition *partition);
void fill_partition(t_stack_ptr stack, struct s_partition *partition, int begin, int end);
struct s_partition *get_top_partition(t_stack_ptr stack);
int get_next_free_partition_id(t_stack_ptr stack);
	
#endif
