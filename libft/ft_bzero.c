/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:29:07 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:09:31 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_bzero fills a memory area with zeros, clearing it.

// What bzero does:
// 	•	bzero sets a block of memory to zero.
// 	•	it makes all the bytes in that memory block equal to 0.

// Convert s (void pointer) into unsigned char * because void * 
// cannot be directly used with array indexing ([])

void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}
