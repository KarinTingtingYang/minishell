/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:37:49 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:04:36 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// ft_calloc
// Allocate a block of memory for an array of nmemb elements,
// each of size bytes, and initialize all bytes to 0.

// Memory allocation function:
// • nmemb: number of elements
// • size: size of each element
// • returns a pointer to memory for nmemb * size bytes, all set to 0

// SIZE_MAX is a constant defined in the C standard library that represents
// the maximum value that a variable of type size_t can hold.

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*array;

	if (nmemb > 0 && size > 0)
	{
		if (nmemb > SIZE_MAX / size)
			return (NULL);
	}
	array = malloc(nmemb * size);
	if (array == NULL)
		return (NULL);
	ft_bzero(array, (nmemb * size));
	return (array);
}
