/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 09:49:16 by makhudon         ###   ########.fr       */
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



// char **tokenize_input(char *line)
// {
// 	int		i = 0;
// 	char	*token = NULL;
// 	char	**tokens = NULL;
// 	int		count = 0;
// 	char	quote = 0;

// 	while (line[i])
// 	{
// 		if (!quote && (line[i] == '\'' || line[i] == '\"'))
// 			quote = line[i]; // entering quote
// 		else if (quote && line[i] == quote)
// 			quote = 0; // exiting quote
// 		else if (!quote && line[i] == ' ')
// 		{
// 			if (token)
// 			{
// 				tokens = save_token(tokens, token, &count);
// 				token = NULL;
// 			}
// 		}
// 		else
// 			token = append_char(token, line[i]);
// 		i++;
// 	}
// 	if (quote)
// 		return print_error("unclosed quote");
// 	if (token)
// 		tokens = save_token(tokens, token, &count);
// 	if (tokens)
// 		tokens[count] = NULL;
// 	return tokens;
// }

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
		else if (!quote && (line[i] == ' ' || line[i] == '|'))
		{
			// Save current token if exists
			if (token)
			{
				tokens = save_token(tokens, token, &count);
				token = NULL;
			}
			// Save '|' as separate token
			if (line[i] == '|')
			{
				tokens = save_token(tokens, strdup("|"), &count);
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