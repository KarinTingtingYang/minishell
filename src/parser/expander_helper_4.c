/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/05 14:00:41 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Copies a string from `str` to `result`, omitting quotes.
 * 
 * This function iterates through the input string `str`, copying characters
 * to `result` while skipping over quote characters. It handles both single
 * and double quotes.
 * @param result The destination string where characters are copied.
 * @param str The source string from which characters are copied.
 */
static void	copy_string_without_quotes(char *result, char *str)
{
	int		i;
	int		j;
	char	quote_char;

	i = 0;
	j = 0;
	quote_char = 0;
	while (str[i] != '\0')
	{
		if (quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
			quote_char = str[i];
		else if (str[i] == quote_char)
			quote_char = 0;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
}

/**
 * @brief Calculates the length of a string without quotes.
 * 
 * This function iterates through the input string, counting characters
 * that are not enclosed in quotes. It handles both single and double quotes.
 * @param str The input string to process.
 * @return The length of the string without quotes.
 */
static size_t	calculate_unquoted_length(const char *str)
{
	size_t	len;
	char	quote_char;
	int		i;

	len = 0;
	quote_char = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
			quote_char = str[i];
		else if (str[i] == quote_char)
			quote_char = 0;
		else
			len++;
		i++;
	}
	return (len);
}

/**
 * @brief Removes quotes from a string and joins the content.
 * 
 * This function calculates the length of the string without quotes,
 * allocates memory for the new string, and copies the content without
 * quotes into it.
 * @param str The input string to process.
 * @return A pointer to the new string without quotes,
 *         or NULL on memory allocation failure.
 */
char	*remove_quotes_and_join(char *str)
{
	char	*result;
	size_t	len;

	if (str == NULL)
		return (NULL);
	len = calculate_unquoted_length(str);
	result = malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (NULL);
	copy_string_without_quotes(result, str);
	return (result);
}
