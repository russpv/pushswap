#ifndef STACK_H
# define STACK_H

# include <stddef.h>
# include <stdio.h>
# include "libft/libft.h"
# include <limits.h>
# include <stdint.h>
# include <stdbool.h>

# define LOGGING true

/* Stack ADT */
/* Forward definitions */
struct s_stack;
struct s_partition;

typedef struct s_stack *t_stack_ptr;
typedef struct s_partition *t_partition_ptr;

/* util */

void		mydebug(const char *format, ...);

/* Stack methods */

t_stack_ptr	create_stack(const char id, const size_t size);
t_stack_ptr	copy_stack(t_stack_ptr);
bool        fill_stack(t_stack_ptr, char**);
void		destroy_stack(t_stack_ptr);

bool		push_stack(t_stack_ptr, int, t_partition_ptr);
long	 	pop_stack(t_stack_ptr);
long		peek_stack(t_stack_ptr);
long        peek_next_stack(t_stack_ptr);
long		peek_bottom(t_stack_ptr);
bool		rotate_stack(t_stack_ptr);
bool		rev_rotate_stack(t_stack_ptr);
bool        swap_stack(t_stack_ptr);

bool		stack_compare(int, t_stack_ptr); /* waht does this do? */
bool        is_sorted_asc(t_stack_ptr);
bool		is_full(t_stack_ptr);
bool        is_sorted_asc_rotationally(t_stack_ptr);
bool        is_stack_extremum(t_stack_ptr, long);

void		print_stack(t_stack_ptr);
void        print_stack_id(t_stack_ptr);
int			get_stack_id(t_stack_ptr);
size_t		get_stack_size(t_stack_ptr);
size_t      get_stack_max_size(t_stack_ptr);
long		get_stack_num(t_stack_ptr, int);
int			get_middle_idx(t_stack_ptr);
size_t      get_partition_count(t_stack_ptr);
int         get_moves_back_to_head(t_stack_ptr);
unsigned long get_entropy(t_stack_ptr, t_partition_ptr);
int         get_stack_idx_at_depth(t_stack_ptr, int);
int         get_insert_position(t_stack_ptr, long);

/* Stack->Partition methods */

t_partition_ptr		create_partition(t_stack_ptr);
t_partition_ptr     copy_partition(t_partition_ptr, t_stack_ptr);
void 				destroy_partition(t_partition_ptr *);
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
long				peek_partition(t_partition_ptr);
#endif
