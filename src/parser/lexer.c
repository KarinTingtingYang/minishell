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
// REDIRECTION DEBUGGING: we can reuse the parse_line function
// as it already handles quoted tokens and spaces.
// we need a helper function to convert t_token ** to char **
// then we call the pase_line function in tokenize_input,
// convert the result to char **, and free the t_token ** afterwards
static char **tokens_to_str_array(t_token **tokens)
{
    int i = 0;
    char **arr;
    
    if (!tokens)
        return (NULL);
    while(tokens[i])
        i++;
    arr = malloc(sizeof(char *) * (i + 1));
    if (!arr)
        return (NULL);
    i = 0;
    while(tokens[i])
    {
        arr[i] = ft_strdup(tokens[i]->value);
        if (!arr[i])
        {
            // Cleanup on allocation failure
            while (--i >= 0)
                free(arr[i]);
            free(arr);
            return (NULL);
        }
        i++;
    }
    arr[i] = NULL;
    return (arr);
}

char **tokenize_input(char *line)
{
    t_token **tokens;
    char    **args;

    // Step 1: Reuse the improved parsing logic from parse_line
    tokens = parse_line(line); 
    
    if (!tokens)
    {
        // If parsing fails (e.g., unclosed quote), parse_line handles the error message.
        // We return an empty, allocated array to prevent crashes in the calling function.
        args = malloc(sizeof(char *));
        if (args)
            args[0] = NULL;
        return (args);
    }

    // Step 2: Convert the token structs to a simple string array
    args = tokens_to_str_array(tokens); 
    
    // Step 3: Free the intermediate token structures to prevent memory leaks
    free_tokens(tokens); 
    
    return (args);
}
// char **tokenize_input(char *line)
// {
// 	int		i = 0;
// 	char	*token = NULL;
// 	char	**tokens = NULL;
// 	int		count = 0;
// 	char	quote = 0;

// 	while (line[i] != '\0')
// 	{
// 		if (quote == 0 && (line[i] == '\'' || line[i] == '\"'))
// 		{
// 			quote = line[i];  // Enter quote mode
// 			i++;              // Skip the quote itself (do not add to token)
// 			continue;
// 		}
// 		else if (quote && line[i] == quote)
// 		{
// 			quote = 0;        // Exit quote mode
// 			i++;              // Skip the quote itself (do not add to token)
// 			continue;
// 		}
// 		else if (quote == 0  && line[i] == ' ')
// 		{
// 			if (token != NULL)
// 			{
// 				tokens = save_token(tokens, token, &count);
// 				token = NULL;
// 			}
// 			i++;  // skip space
// 			continue;
// 		}
// 		else
// 		{
// 			token = append_char(token, line[i]);
// 		}
// 		i++;
// 	}
// 	if (quote != 0)
// 		return print_error("unclosed quote");
// 	if (token != NULL)
// 		tokens = save_token(tokens, token, &count);
// 	if (tokens == NULL)
// 		tokens[count] = NULL;
// 	return tokens;
// }


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

