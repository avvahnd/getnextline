#include "get_next_line.h"

static int	fillbuf(int fd, t_buf *b)
{
	size_t	scan;
	size_t	n;

	if (nl_index(b->data, b->start, b->end) < b->end)
		return (1);
	scan = b->end;
	while (reserve(b, BUFFER_SIZE))
	{
		n = read(fd, b->data + b->end, BUFFER_SIZE);
		if (n <= 0)
			return ((int)n);
		b->end += (size_t)n;
		if (nl_index(b->data, scan, b->end) < b->end)
			return (1);
		scan = b->end;
	}
	return (-1);
}

char	*get_next_line(int fd)
{
	static t_buf	stash;
	char			*line;
	size_t			upto;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (fillbuf(fd, &stash) < 0 || stash.start == stash.end)
		return (drop(&stash));
	upto = nl_index(stash.data, stash.start, stash.end);
	if (upto < stash.end)
		upto++;
	line = takeline(&stash, upto);
	if (!line)
		return (drop(&stash));
	if (stash.start == stash.end)
		drop(&stash);
	return (line);
}
