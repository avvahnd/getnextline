*This project has been created as part of the 42 curriculum by v.*

# get_next_line

## Description

`get_next_line` is a C function that returns the next line of a file descriptor each time it is called, including the trailing `\n` (unless the end of the file is reached without one).

```c
char *get_next_line(int fd);
```

- Returns the line that was read, or `NULL` when there is nothing left to read or an error occurs.
- Works with regular files and with `stdin`.
- Allowed functions: `read`, `malloc`, `free`.
- The read size is set at compile time with `BUFFER_SIZE`.

**Bonus:** the same function, able to handle several file descriptors at once (e.g. fd 3, 4 and 5 read alternately) without losing the reading state of each one, using a single static variable.

## Instructions

Compile with the buffer size of your choice:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

Bonus:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

If `-D BUFFER_SIZE` is not given, the header defines a default of `42`.

Usage example:

```c
int   fd = open("file.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```

## Algorithm

The function keeps its state in one static buffer, a `t_buf` struct:

| Field | Meaning |
|-------|---------|
| `data` | Raw memory block holding the bytes read |
| `start` | Index of the first byte not yet returned |
| `end` | Index right after the last valid byte |
| `cap` | Total allocated size of `data` |

The live bytes are always `data[start .. end)`.

**How it works**

1. `fill_buf` checks whether the buffer already contains a `\n`. If so, it does not call `read` at all.
2. Otherwise, it reserves room for `BUFFER_SIZE` more bytes (`gnl_reserve`) and calls `read` **directly into the buffer**, searching for `\n` only in the newly read bytes, until a newline or EOF is found.
3. `gnl_take_line` copies `data[start .. upto)` into a new string and moves `start` forward. The next call resumes from there.
4. When the buffer is fully consumed, or on error, `gnl_drop` frees everything and resets the state.

**Why this design**

- **Reads the minimum:** it stops reading as soon as a newline is found, and never re-reads leftovers.
- **No repeated concatenation:** Here the block grows by doubling, so reallocations are rare.
- **No temporary buffer:** `read` writes straight into `data`, avoiding an extra allocation, copy and free per read.
- **No string-search over old data:** `scan` marks where the last search ended, so each byte is checked for `\n` only once.
- **Safe on failure:** if `malloc` or `read` fails, `gnl_drop` frees the buffer and the function returns `NULL`, leaving no leaks.
- **Works with any `BUFFER_SIZE`:** the result is identical for 1 or 10,000,000; only the number of `read` calls changes.

**Bonus:** the static variable becomes an array of buffers indexed by file descriptor, `static t_buf stash[MAX_FD]`. Each fd keeps its own `start`/`end`/`cap`, so alternating calls never mix lines. `fd` is also checked against `MAX_FD`.

## Resources

- [`man 2 read`](https://man7.org/linux/man-pages/man2/read.2.html)
- [`man 3 malloc`](https://man7.org/linux/man-pages/man3/malloc.3.html)
- [Storage duration and static variables in C](https://en.cppreference.com/w/c/language/storage_duration)
- [Valgrind quick start](https://valgrind.org/docs/manual/quick-start.html)

**Use of AI:** AI was used to review edge cases (EOF, errors, `BUFFER_SIZE` extremes) and to help write this README. The code itself was written and tested by me.
