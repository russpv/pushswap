#include "hashtable.h"

/* This defines an array of linked lists
 * for indexing
 * Stores multiple entries at an index
 * so no data is lost
 */

static struct s_entry	*g_hasht[HASHSIZE];

static unsigned int	hash(char *s)
{
	unsigned int	hashval;

	hashval = 0;
	while (*s)
		hashval = *s++ + 31 * hashval;
	return (hashval % HASHSIZE);
}

/* Returns node of unique name */
struct s_entry	*lookup(char *s)
{
	struct s_entry	*np;

	np = g_hasht[hash(s)];
	while (NULL != np)
	{
		if (0 == ft_strncmp(s, np->name, -1))
			return (np);
		np = np->next;
	}
	return (NULL);
}

/* Makes new entry the head for the hash bucket */
struct s_entry	*install(char *name, char *data)
{
	struct s_entry	*np;
	unsigned int	hashval;

	np = lookup(name);
	if (NULL == np)
	{
		np = (struct s_entry *)malloc(sizeof(*np));
		np->name = ft_strdup(name);
		if (NULL == np || NULL == np->name)
			return (NULL);
		hashval = hash(name);
		np->next = g_hasht[hashval];
		g_hasht[hashval] = np;
	}
	else
		free((void *)np->data);
	if (NULL != data)
	{
		np->data = ft_strdup(data);
		if (NULL == np->data)
			return (NULL);
	}
	return (np);
}

int	destroy_hasht(void)
{
	struct s_entry	*np;
	struct s_entry	*tmp;
	size_t			i;

	i = -1;
	while (++i < HASHSIZE)
	{
		np = g_hasht[i];
		if (NULL == np)
			continue ;
		while (NULL != np)
		{
			tmp = np->next;
			free(np->name);
			free(np->data);
			np = tmp;
		}
	}
	return (0);
}
