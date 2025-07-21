/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:53:08 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/01 09:53:10 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What is ft_strnstr?
// It searches for a substring (to_find) inside another string (str),
// but only up to a maximum number of characters (len).
// It’s like strstr(), but it stops searching after len characters.

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	size_t	i;
	size_t	j;

	if (*to_find == '\0')
		return ((char *)str);
	i = 0;
	while (str[i] != '\0' && i < len)
	{
		j = 0;
		while (to_find[j] != '\0' && str[i + j] != '\0' && (i + j) < len
			&& str[i + j] == to_find[j])
			j++;
		if (to_find[j] == '\0')
			return ((char *)(str + i));
		i++;
	}
	return (NULL);
}
