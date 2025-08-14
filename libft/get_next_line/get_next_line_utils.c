/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/05 15:56:35 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/14 10:24:31 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

// Adjusted ft_strchr to handle NULL pointer as parameter.
// Returns NULL if NULL pointer is passed into it,
// in order to accommodate the static "storage" string which can be NULL.
char	*gnl_ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	if (!s)
		return (NULL);
	uc = (unsigned char)c;
	while (*s)
	{
		if (*s == uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (NULL);
}

// 1) Adjusted ft_strjoin to handle NULL pointer as parameter,
// so that the function does not return NULL if either parameter is NULL.
// If s1 (storage) is NULL (e.g. 1st time get_next_line is called), 
// simply update s1 (storage) to s2 (read_buffer). 
//
// 2) Adjusted ft_strjoin function prototype (const char *s1 -> char *s1)
// so that s1 can be freed. (const char *) type cannot be freed.
//
// 3) Adjusted ft_strjoin behavior so that each time 
// after creating a new buffer (dest) holding s1 and s2,
// the memory originally allocated to s1 (storage) is freed,
// thus avoiding memory leaks.
char	*gnl_ft_strjoin(char *s1, char const *s2)
{
	size_t	l1;
	size_t	l2;
	char	*dest;
	size_t	i;
	size_t	j;

	l1 = 0;
	l2 = 0;
	if (s1)
		l1 = gnl_ft_strlen(s1);
	if (s2)
		l2 = gnl_ft_strlen(s2);
	dest = malloc(sizeof(char) * (l1 + l2 + 1));
	if (!dest)
		return (free(s1), s1 = NULL, NULL);
	i = 0;
	j = 0;
	while (j < l1)
		dest[i++] = s1[j++];
	j = 0;
	while (j < l2)
		dest[i++] = s2[j++];
	dest[i] = '\0';
	return (free(s1), s1 = NULL, dest);
}

// Note: the below function will return a malloc'd empty string when len == 0.
// To avoid memory leaks, this edge case is checked in the helper function
// ("extract line and update storage") before the below function is called.
char	*gnl_ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	slen;
	size_t	i;

	if (!s)
		return (NULL);
	slen = gnl_ft_strlen(s);
	if (start >= slen)
		len = 0;
	else if (len > slen - start)
		len = slen - start;
	sub = malloc(sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

// helper function that extracts one line (including the "\n" if present) 
// from storage, updates storage with the leftover content, 
// and returns the extracted line.
char	*extract_line_and_update_storage(char **storage)
{
	size_t	i;
	char	*line;
	char	*leftover;

	i = 0;
	while ((*storage)[i] && (*storage)[i] != '\n')
		i++;
	if ((*storage)[i] == '\n')
		i++;
	if (i == 0)
		return (free(*storage), *storage = NULL, NULL);
	line = gnl_ft_substr(*storage, 0, i);
	if (!line)
		return (free(*storage), *storage = NULL, NULL);
	if (!(*storage)[i])
		return (free(*storage), *storage = NULL, line);
	leftover = gnl_ft_substr(*storage, i, gnl_ft_strlen(*storage + i));
	if (!leftover)
	{
		free(line);
		return (free(*storage), *storage = NULL, NULL);
	}
	free(*storage);
	*storage = leftover;
	return (line);
}
