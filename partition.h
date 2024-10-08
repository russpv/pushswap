#ifndef PARTITION_H
# define PARTITION_H

# include "stack.h"

typedef struct s_stack *t_partition_ptr;

t_partition_ptr	create_partition(const t_stack_ptr stack, const int id);
void			destroy_partition(const t_partition_ptr partition);
int				get_median(const t_partition_ptr partition);
void			fill_partition(const t_stack_ptr stack);

#endif
