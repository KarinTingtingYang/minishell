/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 11:05:49 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
