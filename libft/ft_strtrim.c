/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:05:32 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/01 11:23:38 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What is ft_strtrim?
// It creates a new string from s1, but removes any
// characters found in set from the beginning and end of the string.
// Parameters:
// 	•	s1: the original string to be trimmed
// 	•	set: the set of characters to remove from both ends
// Return:
// 	•	A new allocated trimmed string
// 	•	NULL if malloc fails

static int	is_in_set(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;
	size_t	i;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = 0;
	while (s1[start] != '\0' && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in_set(s1[end - 1], set))
		end--;
	trimmed = malloc((end - start + 1) * sizeof(char));
	if (trimmed == NULL)
		return (NULL);
	i = 0;
	while (start < end)
	{
		trimmed[i] = s1[start];
		i++;
		start++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}
