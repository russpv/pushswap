#ifndef STACK_H
# define STACK_H

# include "libft/libft.h"
# include <limits.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>

/* Stack ADT */

/* Forward definitions */

struct						s_stack;
struct						s_partition;

typedef struct s_stack		*t_stack_ptr;
typedef struct s_partition	*t_partition_ptr;

/* util */
// Note: the style guide requires var names, pardon
void						mydebug(const char *v, ...);

/* Stack methods */

t_stack_ptr					create_stack(const char a, const size_t b);
t_stack_ptr					copy_stack(t_stack_ptr a);
bool						fill_stack(t_stack_ptr a, char **b);
void						destroy_stack(t_stack_ptr a);

bool						push_stack(t_stack_ptr a, int b, t_partition_ptr c);
long						pop_stack(t_stack_ptr a);
long						peek_stack(t_stack_ptr a);
long						peek_next_stack(t_stack_ptr a);
long						peek_bottom(t_stack_ptr a);
bool						rotate_stack(t_stack_ptr a);
bool						rev_rotate_stack(t_stack_ptr a);
bool						swap_stack(t_stack_ptr a);

bool						stack_compare(int a, t_stack_ptr b);
bool						is_sorted_asc(t_stack_ptr a);
bool						is_full(t_stack_ptr a);
bool						is_sorted_asc_rotationally(t_stack_ptr a);
bool						is_stack_extremum(t_stack_ptr a, long b);

void						print_stack(t_stack_ptr a);
void						print_stack_id(t_stack_ptr a);
void						print_stack_name(t_stack_ptr a);

int							get_stack_id(t_stack_ptr a);
size_t						get_stack_size(t_stack_ptr a);
size_t						get_stack_max_size(t_stack_ptr a);
long						get_stack_num(t_stack_ptr a, int b);
int							get_middle_idx(t_stack_ptr a);
size_t						get_partition_count(t_stack_ptr a);
int							get_moves_back_to_head(t_stack_ptr a);
unsigned long				get_entropy(t_stack_ptr a, t_partition_ptr b);
int							get_stack_idx_at_depth(t_stack_ptr a, int b);
int							get_insert_position(t_stack_ptr a, long b);

int							get_median(long *a, int *b, size_t c);

/* Stack->Partition methods */

t_partition_ptr				create_partition(t_stack_ptr a);
t_partition_ptr				copy_partition(t_partition_ptr a, t_stack_ptr b);
void						destroy_partition(t_partition_ptr *a);
bool						fill_partition(t_stack_ptr a, t_partition_ptr b,
								int c, int d);

bool						decrement_partition(t_stack_ptr a, int b);
bool						increment_partition(t_stack_ptr a, int b);

t_partition_ptr				get_top_partition(t_stack_ptr a);
int							get_partition_id(t_partition_ptr a);
int							get_next_free_partition_id(t_stack_ptr a);
int							get_top_partition_median(t_stack_ptr a);
size_t						get_partition_size(t_partition_ptr a);
long						peek_partition(t_partition_ptr a);

#endif
