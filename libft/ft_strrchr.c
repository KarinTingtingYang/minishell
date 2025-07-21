/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:29:20 by makhudon          #+#    #+#             */
/*   Updated: 2025/04/30 13:44:35 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_strrchr is a function that:
// Finds the last occurrence of a character c in a string s.

// 	•	s: the string you’re searching in
// 	•	c: the character you’re looking for (passed as int,
//         but treated as char)
// 	•	Returns:
// 		•	a pointer to the last match of c in the string
// 		•	or NULL if not found

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		i--;
	}
	return (NULL);
}
