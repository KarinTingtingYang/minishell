/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/05 15:56:46 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/14 10:23:47 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
/* get_next_line reads from a file descriptor into a buffer ("read_buffer")
and appends it to a static "storage" string (initialised to NULL)
until it encounters a newline or reaches end-of-file. 
It then extracts one line (including the "\n" if present) from storage, 
updates storage with the leftover content, 
and returns the extracted line. 
This process repeats on each call of get_next_line, returning one line at a time
while handling partial reads and leftover data with the "leftover" buffer. */

char	*get_next_line(int fd)
{
	static char	*storage = NULL;
	char		*read_buffer;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	read_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	while (!gnl_ft_strchr(storage, '\n'))
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(read_buffer), free(storage), storage = NULL, NULL);
		if (bytes_read == 0)
			break ;
		read_buffer[bytes_read] = '\0';
		storage = gnl_ft_strjoin(storage, read_buffer);
		if (!storage)
			return (free(read_buffer), NULL);
	}
	free(read_buffer);
	if (!storage || *storage == '\0')
		return (free(storage), storage = NULL, NULL);
	return (extract_line_and_update_storage(&storage));
}

/* // TESTING: txt files
int main(int argc, char **argv)
{
    int     fd;
	char	*line;
	int		line_number;

	if (argc != 2)
	{
		printf("please enter file name after %s\n", argv[0]);
		return (1);
	}

    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }

    printf("Starts reading file: %s\n\n", argv[1]);

    // Read and print lines one by one
	line_number = 1;
	line = get_next_line(fd);
    while (line != NULL)
    {
        printf("Line %d: [%s]\n", line_number, line);
        free(line);
		line = get_next_line(fd);
		line_number++;
    }
    // Close the file
    if (close(fd) < 0)
    {
        perror("Error closing file");
        return (1);
    }

    printf("\nDone reading\n");
    return (0);
} */
//
//
//
// TESTING: STDIN
/* int main(void)
{
    char *line;

    printf("Type something (Ctrl+D to end):\n");

    while ((line = get_next_line(0)) != NULL)  // 0 = stdin
    {
        printf("You typed: [%s]", line);
        free(line);
    }
    printf("\nDone reading from stdin!\n");
    return (0);
} */