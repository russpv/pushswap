#ifndef STACK_H
# define STACK_H

# include <stddef.h>

/* Stack ADT */
typedef struct s_stack *t_stack_ptr;
typedef struct s_partition *t_partition_ptr;

t_stack_ptr	create_stack(const char id, const size_t size);
void		destroy_stack(t_stack_ptr);
void		push_stack(t_stack_ptr, int, t_partition_ptr);
long	 	pop_stack(t_stack_ptr);
long		peek_stack(t_stack_ptr);
void		rotate_stack(t_stack_ptr);
void		rev_rotate_stack(t_stack_ptr);
void		print_stack(t_stack_ptr);
int			stack_compare(int, t_stack_ptr);
int         is_sorted_asc(t_stack_ptr);

/* Partition methods */

t_partition_ptr		create_partition(t_stack_ptr stack);
void 				destroy_partition(t_partition_ptr partition);
void 				fill_partition(t_stack_ptr stack, \
									t_partition_ptr partition, \
									int begin, \
									int end);
t_partition_ptr		get_top_partition(t_stack_ptr stack);
int 				get_next_free_partition_id(t_stack_ptr stack);
int					get_top_partition_median(t_stack_ptr);
size_t				get_partition_size(t_partition_ptr);
	
#endif
