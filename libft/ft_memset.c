/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:07:45 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:10:15 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What does ft_memset do?
// 	•	it takes a block of memory (a pointer s),
// 	•	and fills the first n bytes of that memory with the value c.

// What this function does, in simple words:
// 	•	take a block of memory (starting at pointer s),
// 	•	write the value c into each byte,
// 	•	repeat for n bytes,
// 	•	give back the starting pointer.

// Convert s (void pointer) into unsigned char * because void * 
// cannot be directly used with array indexing ([]).

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;

	i = 0;
	str = (unsigned char *)s;
	while (i < n)
	{
		str[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
