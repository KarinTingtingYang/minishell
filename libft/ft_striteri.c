/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:07:00 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/01 13:15:55 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What ft_striteri does:
// 	•	Takes a string s
// 	•	Applies a function f(index, &char) to each character of s
// 	•	Allows f to modify the characters directly
// This is similar to ft_strmapi, but instead of creating
// a new string, ft_striteri modifies the original string in place.
// Parameters:
// 	•	s: The input string that will be modified
// 	•	f: A function that:
// 		•	Receives the current index
// 		•	Receives a pointer to the character at that index
// 		•	Can modify the character using the pointer

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (s == NULL || f == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		f(i, &s[i]);
		i++;
	}
}
