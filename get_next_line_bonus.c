#include "get_next_line_bonus.h"

static int	fillbuf(int fd, t_buf *b)
{
	size_t	scan;
	ssize_t	n;

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
	static t_buf	stash[MAX_FD];
	char			*line;
	size_t			upto;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	if (fillbuf(fd, &stash[fd]) < 0 || stash[fd].start == stash[fd].end)
		return (drop(&stash[fd]));
	upto = nl_index(stash[fd].data, stash[fd].start, stash[fd].end);
	if (upto < stash[fd].end)
		upto++;
	line = takeline(&stash[fd], upto);
	if (!line)
		return (drop(&stash[fd]));
	if (stash[fd].start == stash[fd].end)
		drop(&stash[fd]);
	return (line);
}
