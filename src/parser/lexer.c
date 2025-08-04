/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 11:46:18 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Tokenizes the input line into an array of tokens.
 * 
 * This function processes the input line, handling quotes and spaces,
 * and returns an array of tokens. It respects quoted strings and
 * handles spaces as delimiters.
 * @param line The input command line string to tokenize.
 * @return A NULL-terminated array of tokens, or NULL on error.
 */
char **tokenize_input(char *line)
{
	int		i = 0;
	char	*token = NULL;
	char	**tokens = NULL;
	int		count = 0;
	char	quote = 0;

	while (line[i])
	{
		if (!quote && (line[i] == '\'' || line[i] == '\"'))
			quote = line[i]; // entering quote
		else if (quote && line[i] == quote)
			quote = 0; // exiting quote
		else if (!quote && line[i] == ' ')
		{
			if (token)
			{
				tokens = save_token(tokens, token, &count);
				token = NULL;
			}
		}
		else
			token = append_char(token, line[i]);
		i++;
	}
	if (quote)
		return print_error("unclosed quote");
	if (token)
		tokens = save_token(tokens, token, &count);
	if (tokens)
		tokens[count] = NULL;
	return tokens;
}

/**
 * @brief Splits the input line by pipe characters into command parts.
 * 
 * This function splits the input line into separate commands based on
 * the pipe character '|'. It handles quoted strings correctly and returns
 * an array of command parts.
 * @param line The input command line string to split.
 * @return A NULL-terminated array of command parts, or NULL on error.
 */
char **split_line_by_pipe(char *line)
{
    int i = 0;
    int start = 0;
    char quote = 0;
    char **parts = NULL;
    int count = 0;

    while (line[i])
    {
        if (!quote && (line[i] == '\'' || line[i] == '\"'))
            quote = line[i];
        else if (quote && line[i] == quote)
            quote = 0;
        else if (!quote && line[i] == '|')
        {
            // Append substring [start, i) as one command
            parts = append_str(parts, line, start, i, &count);
            if (!parts)
                return NULL;
            start = i + 1;
        }
        i++;
    }
    // Append last command substring
    parts = append_str(parts, line, start, i, &count);
    if (!parts)
        return NULL;

    return parts;
}