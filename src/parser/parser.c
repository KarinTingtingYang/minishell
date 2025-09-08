/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 11:37:30 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is a redirection operator.
 * 
 * This function checks if the given string matches any of the
 * redirection operators: ">", ">>", "<", or "<<".
 * 
 * @param s The string to check.
 * @return 1 if the string is a redirection operator, 0 otherwise.
 */
static t_token	*parse_redirection_token(const char **s)
{
	const char	*start;

	start = *s;
	if ((**s == '>' && *(*s + 1) == '>') || (**s == '<' && *(*s + 1) == '<'))
		(*s) += 2;
	else
		(*s)++;
	return (create_token(substr_dup(start, *s - start), NO_QUOTE));
}

/**
 * @brief Parses a word token from the input string.
 * 
 * This function reads characters from the input string until it
 * encounters whitespace or a redirection operator, taking into account
 * quoted substrings. It creates a token with the parsed value and
 * appropriate quote type.
 * 
 * @param s A pointer to the input string pointer. This will be updated
 *          to point to the character after the parsed token.
 * @return A pointer to the created token, or NULL on failure.
 */
static t_token	*parse_word_token(const char **s)
{
	const char		*start;
	char			quote_char;
	t_quote_type	token_quote_type;

	start = *s;
	quote_char = 0;
	token_quote_type = NO_QUOTE;
	if (**s == '\'')
		token_quote_type = SINGLE_QUOTE;
	else if (**s == '"')
		token_quote_type = DOUBLE_QUOTE;
	while (**s)
	{
		if (**s == quote_char)
			quote_char = 0;
		else if (quote_char == 0 && (**s == '\'' || **s == '"'))
			quote_char = **s;
		else if (quote_char == 0 && (**s == ' ' || **s == '\t'
				|| **s == '<' || **s == '>'))
			break ;
		(*s)++;
	}
	return (create_token(substr_dup(start, *s - start), token_quote_type));
}

/**
 * @brief Retrieves the next token from the input string.
 * 
 * This function skips leading whitespace and determines whether the
 * next token is a redirection operator or a word. It then calls the
 * appropriate parsing function to create the token.
 * 
 * @param s A pointer to the input string pointer. This will be updated
 *          to point to the character after the parsed token.
 * @return A pointer to the created token, or NULL if no more tokens are found.
 */
static t_token	*get_next_token(const char **s)
{
	while (**s == ' ' || **s == '\t')
		(*s)++;
	if (**s == '\0')
		return (NULL);
	if (**s == '<' || **s == '>')
		return (parse_redirection_token(s));
	else
		return (parse_word_token(s));
}

/**
 * @brief Fills an array with tokens parsed from the input string.
 * 
 * This function iterates to parse the specified number of tokens
 * from the input string and fills the provided array with the
 * created tokens. If any token creation fails, it frees all
 * previously created tokens and returns -1.
 * 
 * @param s The input string to parse.
 * @param tokens The array to fill with parsed tokens.
 * @param token_count The number of tokens to parse.
 * @return 0 on success, -1 on failure.
 */
static int	fill_tokens_array(const char *s, t_token **tokens, int token_count)
{
	int	i;

	i = 0;
	while (i < token_count)
	{
		tokens[i] = get_next_token(&s);
		if (tokens[i] == NULL)
		{
			free_tokens(tokens);
			return (-1);
		}
		i++;
	}
	tokens[i] = NULL;
	return (0);
}

/**
 * @brief Parses an input line into an array of tokens.
 * 
 * This function checks for null or empty input, verifies that all
 * quotes are properly closed, counts the number of tokens, allocates
 * memory for the token array, and fills it with parsed tokens.
 * 
 * @param line The input line to parse.
 * @return A pointer to the array of tokens, or NULL on failure.
 */
t_token	**parse_line(char *line, t_process_data *pd)
{
	int		token_count;
	t_token	**tokens;

	if (line == NULL || *line == '\0')
		return (NULL);
	if (!quotes_are_closed(line, pd))
	{
		if (pd)
			pd->last_exit_status = 2;
		return (NULL);
	}
	token_count = count_tokens(line);
	tokens = malloc(sizeof(t_token *) * (token_count + 1));
	if (tokens == NULL)
		return (NULL);
	if (fill_tokens_array(line, tokens, token_count) == -1)
	{
		free(tokens);
		return (NULL);
	}
	return (tokens);
}
