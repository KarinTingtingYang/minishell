/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 09:31:39 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

static int	count_words(const char *str)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str != '\0')
	{
		if (!is_space(*str) && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (is_space(*str))
			in_word = 0;
		str++;
	}
	return (count);
}

/**
 * @brief Creates an array of strings from a given string,
 *        splitting by whitespace.
 * 
 * This function allocates memory for an array of strings and fills it with
 * words from the input string, splitting by spaces. The resulting array is
 * NULL-terminated.
 * @param str The input string to split.
 * @return A dynamically allocated array of strings, or NULL on failure.
 */
static void	create_split_array(char **result, char *str)
{
	int			i;
	const char	*start;
	size_t		len;

	i = 0;
	while (*str != '\0')
	{
		while (*str && is_space(*str))
			str++;
		if (*str == '\0')
			break ;
		start = str;
		len = 0;
		while (*str && !is_space(*str))
		{
			str++;
			len++;
		}
		result[i++] = ft_strndup(start, len);
	}
	result[i] = NULL;
}

/**
 * @brief Splits a string into an array of words based on whitespace.
 * 
 * This function allocates memory for an array of strings and fills it with
 * words from the input string, splitting by spaces. The resulting array is
 * NULL-terminated.
 * @param str The input string to split.
 * @return A dynamically allocated array of strings, or NULL on failure.
 */
char	**ft_split_whitespace(char *str)
{
	int		words;
	char	**result;

	if (str == NULL)
		return (NULL);
	words = count_words(str);
	result = malloc((words + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	create_split_array(result, str);
	return (result);
}
