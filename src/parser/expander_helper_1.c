/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:57:24 by makhudon         ###   ########.fr       */
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
 * @brief Creates an array of strings by splitting the input string on
 * whitespace.
 * 
 * This function fills the provided array with words extracted from the
 * input string, splitting by spaces. It assumes that the array has enough
 * space to hold all words plus a NULL terminator.
 * @param result The array to fill with split words.
 * @param str The input string to split.
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
 * This function counts the number of words in the input string, allocates
 * an array of strings, and fills it by splitting the input on whitespace.
 * The resulting array is NULL-terminated.
 * @param str The input string to split.
 * @return A pointer to the newly allocated array of strings,
 *         or NULL on failure.
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

/**
 * @brief Removes outer quotes from a string if they match.
 * 
 * This function checks if the input string starts and ends with the same
 * quote character (single or double). If so, it removes those quotes and
 * returns a new string. If not, it returns the original string.
 * @param str The input string to process.
 * @return A pointer to the new string without outer quotes,
 *         or the original string if no quotes were removed.
 */
char	*remove_outer_quotes(char *str)
{
	size_t	len;
	char	*result;

	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '\'' && str[len - 1] == '\'')
			|| (str[0] == '"' && str[len - 1] == '"')))
	{
		if (len == 2)
			result = ft_strdup("");
		else
			result = ft_substr(str, 1, len - 2);
		if (result == NULL)
		{
			free(str);
			return (NULL);
		}
		free(str);
		return (result);
	}
	return (str);
}
