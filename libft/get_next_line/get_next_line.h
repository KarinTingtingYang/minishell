/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/05 15:56:50 by tiyang        #+#    #+#                 */
/*   Updated: 2025/05/13 12:29:01 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h> // size_t
# include <unistd.h> // read
# include <stdlib.h> // malloc, free
// for testing:
// # include <fcntl.h>    // open
// # include <stdio.h>    // printf, perror

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 5
# endif

size_t	gnl_ft_strlen(const char *s);
char	*gnl_ft_strchr(const char *s, int c);
char	*gnl_ft_strjoin(char *s1, char const *s2);
char	*gnl_ft_substr(char const *s, unsigned int start, size_t len);
char	*extract_line_and_update_storage(char **storage);
char	*get_next_line(int fd);

#endif
//cc -Wall -Wextra -Werror -D BUFFER_SIZE=? <file name>.c
