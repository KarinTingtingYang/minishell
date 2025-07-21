/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 15:02:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/05 12:16:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strdu(const char *src, int length)
{
	char	*copy;
	int		i;

	copy = (char *)malloc(length + 1);
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < length)
	{
		copy[i] = src[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

static char	**write_words(const char *str, char c, char **result)
{
	const char	*start;
	int			i;
	int			length;

	i = 0;
	while (*str != '\0')
	{
		if (*str != c)
		{
			start = str;
			length = 0;
			while (*str != '\0' && *str != c)
			{
				str++;
				length++;
			}
			result[i++] = ft_strdu(start, length);
		}
		else
			str++;
	}
	result[i] = NULL;
	return (result);
}

static int	count_words(const char *str, char c)
{
	int	count;
	int	word_started;

	count = 0;
	word_started = 0;
	while (*str != '\0')
	{
		if (*str != c && word_started == 0)
		{
			word_started = 1;
			count++;
		}
		else if (*str == c)
			word_started = 0;
		str++;
	}
	return (count);
}

char	**ft_split(const char *str, char c)
{
	char	**result;

	if (str == NULL)
		return (NULL);
	result = malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (result == NULL)
		return (NULL);
	return (write_words(str, c, result));
}
