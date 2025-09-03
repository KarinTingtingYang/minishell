/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:04:10 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/03 11:15:20 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Duplicates a substring from a given start pointer with
 *        specified length.
 *
 * This function allocates memory for a new string, copies the substring
 * of the specified length starting from the given pointer, and
 * null-terminates it.
 *
 * @param start Pointer to the start of the substring.
 * @param len Length of the substring to duplicate.
 * @return A newly allocated string containing the duplicated substring,
 *         or NULL if memory allocation fails.
 */
char	*substr_dup(const char *start, size_t len)
{
	char	*result;
	size_t	i;

	result = malloc(len + 1);
	if (result == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = start[i];
		i++;
	}
	result[len] = '\0';
	return (result);
}

/**
 * @brief Skips over a token in the input string.
 *
 * This function advances the pointer past a token, which can be
 * a redirection operator or a sequence of non-whitespace characters.
 * It properly handles quoted sections to avoid splitting tokens
 * within quotes.
 *
 * @param s Pointer to the current position in the input string.
 * @return Pointer advanced past the current token.
 */
static const char	*skip_token(const char *s)
{
	char	quote;

	quote = 0;
	if (*s == '<' || *s == '>')
	{
		if ((*s == '<' && *(s + 1) == '<') || (*s == '>' && *(s + 1) == '>'))
			return (s + 2);
		return (s + 1);
	}
	while (*s != '\0')
	{
		if (quote == 0 && (*s == '\'' || *s == '\"'))
			quote = *s;
		else if (quote && *s == quote)
			quote = 0;
		else if (quote == 0 && (*s == ' ' || *s == '\t'
				|| *s == '<' || *s == '>'))
			break ;
		s++;
	}
	return (s);
}

/**
 * @brief Counts the number of tokens in a string.
 *
 * This function iterates through the input string, counting
 * sequences of non-whitespace characters as tokens. It properly
 * handles quoted sections to avoid splitting tokens within quotes.
 *
 * @param s The input string to analyze.
 * @return The number of tokens found in the string.
 */
int	count_tokens(const char *s)
{
	int	count;

	count = 0;
	while (*s != '\0')
	{
		while (*s == ' ' || *s == '\t')
			s++;
		if (*s == '\0')
			break ;
		count++;
		s = skip_token(s);
	}
	return (count);
}

/**
 * @brief Creates a new token with the given value and quote type.
 *
 * This function allocates memory for a new token structure,
 * assigns the provided value and quote type, and returns
 * a pointer to the newly created token.
 *
 * @param value The string value of the token.
 * @param quote_type The quote type of the token
 *                   (NO_QUOTE, SINGLE_QUOTE, DOUBLE_QUOTE).
 * @return A pointer to the newly created token,
 *         or NULL if memory allocation fails.
 */
t_token	*create_token(char *value, t_quote_type quote_type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->value = value;
	token->quote = quote_type;
	return (token);
}
