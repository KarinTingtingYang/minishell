/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:55:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:07:05 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_memcpy copies n bytes from a memory area src to a memory area dest.

// What ft_memcpy does:
// 	•	it copies byte by byte.
// 	•	it does not check for memory overlap — if dest and src overlap, 
//      the behavior is undefined.
// 	•	after copying, it returns dest.

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*destination;
	const unsigned char	*source;

	if (!dest && !src)
		return (NULL);
	i = 0;
	destination = (unsigned char *)dest;
	source = (const unsigned char *)src;
	while (i < n)
	{
		destination[i] = source[i];
		i++;
	}
	return (dest);
}
