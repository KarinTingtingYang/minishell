/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:09 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 10:13:52 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static void	update_quote_status(char c, char *quote)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

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

char	**split_line_by_pipe(char *line)
{
	t_append_info	split_info;

	split_info.array = NULL;
	split_info.count = 0;
	return (process_line_and_split(line, &split_info));
}
