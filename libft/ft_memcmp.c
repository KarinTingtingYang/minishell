/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:45:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/02 13:24:26 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What does ft_memcmp do?
// ft_memcmp compares the first n bytes of two memory blocks (s1 and s2):
// 	•	If the memory is the same, it returns 0.
// 	•	If there’s a difference, it returns the difference between
// 		the first mismatched bytes.
// 	•	It’s similar to strcmp, but for any kind of data, not just strings.

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*bytes1;
	const unsigned char	*bytes2;

	bytes1 = (const unsigned char *)s1;
	bytes2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (bytes1[i] != bytes2[i])
			return (bytes1[i] - bytes2[i]);
		i++;
	}
	return (0);
}
