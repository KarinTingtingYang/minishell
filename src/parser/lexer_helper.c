/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:44:35 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/06 14:11:02 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Duplicates an array of token values into a new array of strings.
 *
 * This function creates a new array of strings by duplicating the
 * `value` field from each token in the input array. The new array is
 * NULL-terminated.
 *
 * @param tokens An array of token pointers.
 * @param len The number of tokens in the array.
 * @return A newly allocated array of strings, or NULL on failure.
 */
char	**duplicate_tokens_to_array(t_token **tokens, int len)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (len + 1));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		array[i] = ft_strdup(tokens[i]->value);
		if (!array[i])
		{
			while (--i >= 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}

/**
 * @brief Appends a character to the end of a string.
 *
 * This function creates a new string that is the result of appending
 * the specified character to the end of the input string. The original
 * string is freed.
 *
 * @param str The original string (can be NULL).
 * @param c The character to append.
 * @return A newly allocated string with the character appended,
 *         or NULL on failure.
 */
char	*append_char(char *str, char c)
{
	int		len;
	char	*new_str;

	if (str != NULL)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	if (str != NULL)
	{
		ft_strlcpy(new_str, str, len + 1);
		free(str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

/**
 * @brief Appends a substring from a line to an array of strings.
 *
 * This function extracts a substring from the given line using the
 * specified start and end indices, and appends it to the array
 * contained in the t_append_info structure. The array is resized
 * to accommodate the new entry.
 *
 * @param info A pointer to the t_append_info structure containing
 *             the array and its count.
 * @param line The original line from which to extract the substring.
 * @param start The starting index of the substring (inclusive).
 * @param end The ending index of the substring (exclusive).
 * @return A pointer to the updated array, or NULL on failure.
 */
char	**append_str(t_append_info *info, char *line, int start, int end)
{
	int		len;
	char	*substr;
	char	**new_arr;

	len = end - start;
	substr = malloc(len + 1);
	if (substr == NULL)
		return (NULL);
	ft_strlcpy(substr, line + start, len + 1);
	new_arr = realloc(info->array, sizeof(char *) * (info->count + 2));
	if (new_arr == NULL)
	{
		free(substr);
		free_split(info->array);//DEBUG
		return (NULL);
	}
	new_arr[info->count] = substr;
	info->count++;
	new_arr[info->count] = NULL;
	return (new_arr);
}

/**
 * @brief Frees an array of tokens and their values.
 *
 * This function frees each token's value and the token itself,
 * followed by freeing the array of token pointers.
 *
 * @param tokens An array of token pointers to free.
 */
void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (tokens == NULL)
		return ;
	while (tokens[i] != NULL)
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
