/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:58:39 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/01 10:11:01 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What does ft_strdup do?
// ft_strdup creates a duplicate (copy) of a string:
// 	1.	Calculates the length of the string.
// 	2.	Allocates enough memory for a copy.
// 	3.	Copies each character into that new memory.
// 	4.	Returns the new pointer

char	*ft_strdup(const char *src)
{
	size_t	length;
	int		i;
	char	*copy;

	length = 0;
	while (src[length] != '\0')
	{
		length++;
	}
	copy = (char *)malloc(length + 1);
	if (copy == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (src[i] != '\0')
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
