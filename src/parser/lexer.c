/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 15:05:33 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Tokenizes the input line into an array of strings.
 * 
 * This function processes the input line, handling quoted sections and spaces.
 * It returns an array of tokens, where each token is a command or argument.
 * If there is an unclosed quote, it prints an error message and returns NULL.
 * @param line The input line to tokenize.
 * @return An array of strings (tokens), or NULL on error.
 */
char **tokenize_input(char *line)
{
	int		i = 0;
	char	*token = NULL;
	char	**tokens = NULL;
	int		count = 0;
	char	quote = 0;

	while (line[i] != '\0')
	{
		if (quote == 0 && (line[i] == '\'' || line[i] == '\"'))
		{
			quote = line[i];  // Enter quote mode
			i++;              // Skip the quote itself (do not add to token)
			continue;
		}
		else if (quote && line[i] == quote)
		{
			quote = 0;        // Exit quote mode
			i++;              // Skip the quote itself (do not add to token)
			continue;
		}
		else if (quote == 0  && line[i] == ' ')
		{
			if (token != NULL)
			{
				tokens = save_token(tokens, token, &count);
				token = NULL;
			}
			i++;  // skip space
			continue;
		}
		else
		{
			token = append_char(token, line[i]);
		}
		i++;
	}
	if (quote != 0)
		return print_error("unclosed quote");
	if (token != NULL)
		tokens = save_token(tokens, token, &count);
	if (tokens == NULL)
		tokens[count] = NULL;
	return tokens;
}


/**
 * @brief Splits a line into commands separated by pipes.
 * 
 * This function splits the input line into an array of strings, where each
 * string is a command that was separated by a pipe ('|'). It handles quoted
 * sections correctly and reallocates memory as necessary.
 * @param line The input line to split.
 * @return An array of strings representing the commands, or NULL on failure.
 */
char **split_line_by_pipe(char *line)
{
    int i = 0;
    int start = 0;
    char quote = 0;
    char **parts = NULL;
    int count = 0;

    while (line[i] != '\0')
    {
        if (quote == 0 && (line[i] == '\'' || line[i] == '\"'))
            quote = line[i];
        else if (quote && line[i] == quote)
            quote = 0;
        else if (quote == 0 && line[i] == '|')
        {
            // Append substring [start, i) as one command
            parts = append_str(parts, line, start, i, &count);
            if (parts == NULL)
                return NULL;
            start = i + 1;
        }
        i++;
    }
    // Append last command substring
    parts = append_str(parts, line, start, i, &count);
    if (parts == NULL)
        return NULL;
    return parts;
}

