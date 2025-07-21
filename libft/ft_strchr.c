/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:24:13 by makhudon          #+#    #+#             */
/*   Updated: 2025/04/30 13:43:16 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What ft_strchr does:
// It searches for the first occurrence of a character c in the string s, and:
// 	• If found → returns a pointer to that character in the string.
// 	• If not found → returns NULL (which is 0).
// 	• If you're searching for the null terminator ('\0') → 
// 	  it should return a pointer to the end of the string.

char	*ft_strchr(const char *s, int c)
{
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}
