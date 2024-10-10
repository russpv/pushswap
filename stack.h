#ifndef STACK_H
# define STACK_H

# include <stddef.h>
# include <stdio.h>
# include "libft/libft.h"
# include <limits.h>
# include <stdbool.h>

/* Stack ADT */
/* Forward definitions */
struct s_stack;
struct s_partition;

typedef struct s_stack *t_stack_ptr;
typedef struct s_partition *t_partition_ptr;

/* Stack methods */

t_stack_ptr	create_stack(const char id, const size_t size);
bool        fill_stack(t_stack_ptr, char**);
void		destroy_stack(t_stack_ptr);
bool		push_stack(t_stack_ptr, int, t_partition_ptr);
long	 	pop_stack(t_stack_ptr);
long		peek_stack(t_stack_ptr);
bool		rotate_stack(t_stack_ptr);
bool		rev_rotate_stack(t_stack_ptr);
void		print_stack(t_stack_ptr);
bool		stack_compare(int, t_stack_ptr);
bool        is_sorted_asc(t_stack_ptr);
int			get_stack_id(t_stack_ptr);

/* Stack->Partition methods */

t_partition_ptr		create_partition(t_stack_ptr);
void 				destroy_partition(t_partition_ptr);
bool 				fill_partition(t_stack_ptr stack, \
									t_partition_ptr partition, \
									int begin, \
									int end);

bool                decrement_partition(t_stack_ptr, int);
bool                increment_partition(t_stack_ptr, int);

t_partition_ptr		get_top_partition(t_stack_ptr);
int					get_partition_id(t_partition_ptr);
int 				get_next_free_partition_id(t_stack_ptr);
int					get_top_partition_median(t_stack_ptr);
size_t				get_partition_size(t_partition_ptr);
	
#endif
