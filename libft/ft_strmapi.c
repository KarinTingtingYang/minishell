/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:20:14 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/01 12:47:52 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// This function:
// 	•	Takes a string s
// 	•	Applies a function f(index, char) to each character of s
// 	•	Returns a new string with the transformed characters

// The original string s is not modified.
// Function signature
// 	•	s: the input string
// 	•	f: a function that takes:
// 	•	an unsigned int (the index of the character)
// 	•	a char (the character itself)
// 	•	and returns a new character

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	unsigned int	length;
	char			*result;

	if (s == NULL || f == NULL)
		return (NULL);
	length = ft_strlen(s);
	result = malloc((length + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < length)
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
