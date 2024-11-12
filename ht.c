#include "hashtable.h"

/* This defines an array of linked lists
 * for indexing
 * Stores multiple entries at an index
 * so no data is lost
 * Inspired by K&R Ch 6.6
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

static int	_install_data(struct s_entry *np, char *data)
{
	if (NULL != data)
	{
		np->data = ft_strdup(data);
		if (NULL == np->data)
			return (-1);
	}
	else
		np->data = NULL;
	return (0);
}

/* Makes new entry the head for the hash bucket 
 * Returns NULL if name is already present
 */
struct s_entry	*install(char *name, char *data)
{
	struct s_entry	*np;
	unsigned int	hashval;

	if (NULL == name)
		return (NULL);
	np = lookup(name);
	if (NULL == np)
	{
		np = (struct s_entry *)malloc(sizeof(*np));
		if (NULL == np)
			return (NULL);
		np->name = ft_strdup(name);
		if (NULL == np->name)
			return (NULL);
		hashval = hash(name);
		np->next = g_hasht[hashval];
		g_hasht[hashval] = np;
		if (-1 == _install_data(np, data))
			return (NULL);
	}
	else
		return (NULL);
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
			if (NULL != np->data)
				free(np->data);
			free(np);
			np = tmp;
		}
	}
	return (0);
}
