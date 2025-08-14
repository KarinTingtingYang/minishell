/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 11:04:09 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

t_token	**parse_line(char *line)
{
	int		token_count;
	t_token	**tokens;

	if (line == NULL || *line == '\0')
		return (NULL);
	token_count = count_tokens(line);
	tokens = malloc(sizeof(t_token *) * (token_count + 1));
	if (tokens == NULL)
		return (NULL);
	if (fill_tokens_array(line, tokens, token_count) == -1)
		return (NULL);
	return (tokens);
}
