/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 10:05:20 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **save_token(char **tokens, char *token, int *count)
{
	char	**new_tokens;
	int		i = 0;

	new_tokens = malloc(sizeof(char *) * (*count + 2));
	if (!new_tokens)
		return (NULL);
	while (i < *count)
	{
		new_tokens[i] = tokens[i];
		i++;
	}
	new_tokens[i++] = token;
	new_tokens[i] = NULL;
	free(tokens);
	(*count)++;
	return (new_tokens);
}

char **print_error(const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (NULL);
}

char *append_char(char *str, char c)
{
	int		len = str ? ft_strlen(str) : 0;
	char	*new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	if (str)
	{
		ft_strlcpy(new_str, str, len + 1);
		free(str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}



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


static char **append_str(char **arr, char *line, int start, int end, int *count)
{
    int len = end - start;
    char *substr = malloc(len + 1);
    if (!substr)
        return NULL;
    strncpy(substr, line + start, len);
    substr[len] = '\0';

    char **new_arr = realloc(arr, sizeof(char *) * (*count + 2));
    if (!new_arr)
    {
        free(substr);
        return NULL;
    }
    new_arr[*count] = substr;
    (*count)++;
    new_arr[*count] = NULL;
    return new_arr;
}

// Splits input line by pipes '|' respecting quotes, returns NULL on error
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