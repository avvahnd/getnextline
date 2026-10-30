#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define MAX_FD 42

typedef struct s_buf
{
	char	*data;
	size_t	start;
	size_t	end;
	size_t	cap;
}	t_buf;

char	*get_next_line(int fd);
size_t	nl_index(const char *data, size_t from, size_t end);
int		reserve(t_buf *b, size_t n);
char	*takeline(t_buf *b, size_t upto);
char	*drop(t_buf *b);

#endif