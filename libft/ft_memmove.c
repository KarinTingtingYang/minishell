/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:27:12 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:08:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_memmove copies n bytes from one memory area (src) to another (dest) —
// just like memcpy, but it is safe when memory overlaps.

// How does it work?
// check if the destination starts after the source. If yes:
// 	•	copy backward (from the end), to avoid overwriting 
//      bytes you haven’t read yet.
// if there’s no overlap or dest < src:
// 	•	copy forward as usual.

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*destination;
	const unsigned char	*source;

	if (!dest && !src)
		return (NULL);
	destination = (unsigned char *)dest;
	source = (const unsigned char *)src;
	if (destination > source)
	{
		while (n-- > 0)
			destination[n] = source[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			destination[i] = source[i];
			i++;
		}
	}
	return (dest);
}
