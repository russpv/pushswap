#include "checker.h"

char	*get_next_line(int fd)
{
	char			c;
	char			buf[BUFFER_SIZE];
	int				bytes;
	unsigned long	i;

	i = 0;
	c = 0;
	while (i < sizeof(buf) - 2)
	{
		bytes = read(fd, &c, 1);
		if (bytes < 0)
			return (NULL);
		if (0 == bytes || '\n' == c)
			break ;
		buf[i++] = c;
	}
	if (0 == i)
		return (NULL);
	buf[i] = '\0';
	return (ft_strdup(buf));
}
