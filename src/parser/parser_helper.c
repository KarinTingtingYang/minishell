/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_helper.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/21 11:05:27 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/27 12:59:13 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_redirect(const char *s)
{
    if (ft_strncmp(s, ">>", 3) == 0)
        return (1);
    if (ft_strncmp(s, "<<", 3) == 0)
        return (1);
    if (ft_strncmp(s, ">", 2) == 0)
        return (1);
    if (ft_strncmp(s, "<", 2) == 0)
        return (1);
    return (0);
}

int validate_redirect_syntax(t_token **tokens)
{
    int     i;
    char    *unexpected;
    char    *tmp;
    char    *msg;

    i = 0;
    while (tokens && tokens[i])
    {
        if (is_redirect(tokens[i]->value))
        {
            if (tokens[i + 1] == NULL || is_redirect(tokens[i + 1]->value))
            {
                if (tokens[i + 1] != NULL)
                    unexpected = tokens[i + 1]->value;
                else
                    unexpected = "newline";
                tmp = ft_strjoin("syntax error near unexpected token `", unexpected);
                if (tmp == NULL)
                    return (0);
                msg = ft_strjoin(tmp, "'");
                free(tmp);
                if (msg == NULL)
                    return (0);
                ft_error(NULL, msg);
                free(msg);
                return (0);
            }
        }
        i++;
    }
    return (1);
}


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
	while (*s)
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

int	count_tokens(const char *s)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == ' ' || *s == '\t')
			s++;
		if (!*s)
			break ;
		count++;
		s = skip_token(s);
	}
	return (count);
}

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

/**
 * @brief Checks if all quotes in a line are properly closed.
 *
 * This function iterates through the input string to ensure that every
 * single and double quote is part of a matching pair.
 * @param line The command line string to validate.
 * @return 1 if all quotes are closed, 0 otherwise.
 */
int	quotes_are_closed(const char *line)
{
	char	quote_char;

	quote_char = 0;
	while (*line)
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (*line == quote_char)
			quote_char = 0;
		line++;
	}
	if (quote_char != 0)
	{
		// You can use your project's error printing function.
		// Using a standard library function here for simplicity.
		ft_error("", "syntax error (unclosed quote)");
		return (0);
	}
	return (1);
}

/**
 * @brief Counts the number of unquoted heredoc delimiters (<<) in a line.
 * @param line The command line string to check.
 * @return The total number of heredocs.
 */
int	count_heredocs(const char *line)
{
	int		count;
	char	quote_char;

	count = 0;
	quote_char = 0;
	while (line && *line)
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (*line == quote_char)
			quote_char = 0;
		if (quote_char == 0 && *line == '<' && *(line + 1) == '<')
		{
			count++;
			line++;
		}
		line++;
	}
	return (count);
}