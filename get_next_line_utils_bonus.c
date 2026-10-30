#include "get_next_line_bonus.h"

size_t	nl_index(const char *data, size_t from, size_t end)
{
	while (from < end && data[from] != '\n')
		from++;
	return (from);
}

int	reserve(t_buf *b, size_t n)
{
	char	*newbuff;
	size_t	live;
	size_t	i;

	if (b->cap - b->end >= n)
		return (1);
	live = b->end - b->start;
	newbuff = malloc((live + n) * 2);
	if (!newbuff)
		return (0);
	i = 0;
	while (i < live)
	{
		newbuff[i] = b->data[b->start + i];
		i++;
	}
	free(b->data);
	b->data = newbuff;
	b->cap = (live + n) * 2;
	b->start = 0;
	b->end = live;
	return (1);
}

char	*takeline(t_buf *b, size_t upto)
{
	char	*line;
	size_t	len;
	size_t	i;

	len = upto - b->start;
	line = malloc(len + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = b->data[b->start + i];
		i++;
	}
	line[len] = '\0';
	b->start = upto;
	return (line);
}

char	*drop(t_buf *b)
{
	free(b->data);
	b->data = NULL;
	b->start = 0;
	b->end = 0;
	b->cap = 0;
	return (NULL);
}
