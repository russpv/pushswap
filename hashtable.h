#ifndef HASHTABLE_H
# define HASHTABLE_H

# include "libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define HASHSIZE 101

struct				s_entry
{
	struct s_entry	*next;
	char			*name;
	char			*data;
};

struct s_entry		*lookup(char *s);
struct s_entry		*install(char *name, char *data);
int					destroy_hasht(void);

#endif
