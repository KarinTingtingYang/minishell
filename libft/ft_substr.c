/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:11:50 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:44:25 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What does ft_substr do:
// Create a substring from a given string s, starting
// at position start, with a maximum length of len.

// char const *s
// 	•	A pointer to a constant character array,
// 		which means a string input.
// 	•	const means the function promises not to
// 		modify the original string.
// unsigned int start
// 	•	This is the starting index of the substring
// 		you want to extract from the string s.
// 	•	It must be unsigned, so it cannot be negative.
// size_t len
// 	•	The maximum number of characters to include in 
// 		the substring.
// 	•	size_t is the standard type used for sizes and lengths.

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;
	size_t	source_len;
	size_t	copy_len;

	if (s == NULL)
		return (NULL);
	source_len = ft_strlen(s);
	if (start >= source_len)
		return (ft_strdup(""));
	copy_len = source_len - start;
	if (copy_len > len)
		copy_len = len;
	substring = malloc(copy_len + 1);
	if (substring == NULL)
		return (NULL);
	ft_strlcpy(substring, s + start, copy_len + 1);
	return (substring);
}
