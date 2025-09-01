/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 15:37:37 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Converts an array of tokens to an array of strings.
 *
 * This function counts the number of tokens in the input array and
 * uses `duplicate_tokens_to_array` to create a new array of strings
 * containing the token values.
 *
 * @param tokens An array of token pointers.
 * @return A newly allocated array of strings, or NULL on failure.
 */
static char	**tokens_to_str_array(t_token **tokens)
{
	int	i;

	if (tokens == NULL)
		return (NULL);
	i = 0;
	while (tokens[i] != NULL)
		i++;
	return (duplicate_tokens_to_array(tokens, i));
}

/**
 * @brief Tokenizes an input line into an array of strings.
 *
 * This function uses the lexer to parse the input line into tokens,
 * then converts those tokens into an array of strings. It frees the
 * tokens after conversion.
 *
 * @param line The input line to tokenize.
 * @return A newly allocated array of strings, or NULL on failure.
 */
char	**tokenize_input(char *line)
{
	t_token	**tokens;
	char	**args;

	tokens = parse_line(line);
	if (!tokens)
	{
		args = malloc(sizeof(char *));
		if (args)
			args[0] = NULL;
		return (args);
	}
	args = tokens_to_str_array(tokens);
	free_tokens(tokens);
	return (args);
}

/**
 * @brief Updates the quote status based on the current character.
 *
 * This function modifies the quote status variable based on the
 * encountered character. It toggles the quote status when entering
 * or exiting quoted sections.
 *
 * @param c The current character being processed.
 * @param quote A pointer to the current quote status variable.
 */
static void	update_quote_status(char c, char *quote)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

/**
 * @brief Processes a line to split it by unquoted pipe characters.
 *
 * This function iterates through the input line, tracking quote status,
 * and splits the line into segments at unquoted pipe characters. It uses
 * `append_str` to build the resulting array of strings.
 *
 * @param line The input line to process.
 * @param info A pointer to a t_append_info structure for managing the array.
 * @return A newly allocated array of strings, or NULL on failure.
 */
static char	**process_line_and_split(char *line, t_append_info *info)
{
	int		i;
	int		start;
	char	quote;

	i = 0;
	start = 0;
	quote = 0;
	while (line[i] != '\0')
	{
		update_quote_status(line[i], &quote);
		if (quote == 0 && line[i] == '|')
		{
			info->array = append_str(info, line, start, i);
			if (info->array == NULL)
				return (NULL);
			start = i + 1;
		}
		i++;
	}
	info->array = append_str(info, line, start, i);
	if (info->array == NULL)
		return (NULL);
	return (info->array);
}

/**
 * @brief Splits an input line by unquoted pipe characters.
 *
 * This function initializes the necessary structures and calls
 * `process_line_and_split` to perform the actual splitting of the
 * input line. It returns the resulting array of strings.
 *
 * @param line The input line to split.
 * @return A newly allocated array of strings, or NULL on failure.
 */
char	**split_line_by_pipe(char *line)
{
	t_append_info	split_info;

	split_info.array = NULL;
	split_info.count = 0;
	return (process_line_and_split(line, &split_info));
}
