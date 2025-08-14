/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:44:35 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 10:15:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**duplicate_tokens_to_array(t_token **tokens, int len)
{
	char	**array;
	int		i;

	array = malloc(sizeof(char *) * (len + 1));
	if (array == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		array[i] = ft_strdup(tokens[i]->value);
		if (!array[i])
		{
			while (--i >= 0)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
	}
	array[i] = NULL;
	return (array);
}

char	*append_char(char *str, char c)
{
	int		len;
	char	*new_str;

	if (str != NULL)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	if (str != NULL)
	{
		ft_strlcpy(new_str, str, len + 1);
		free(str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char	**append_str(t_append_info *info, char *line, int start, int end)
{
	int		len;
	char	*substr;
	char	**new_arr;

	len = end - start;
	substr = malloc(len + 1);
	if (substr == NULL)
		return (NULL);
	ft_strlcpy(substr, line + start, len + 1);
	new_arr = realloc(info->array, sizeof(char *) * (info->count + 2));
	if (new_arr == NULL)
	{
		free(substr);
		return (NULL);
	}
	new_arr[info->count] = substr;
	info->count++;
	new_arr[info->count] = NULL;
	return (new_arr);
}

void	free_tokens(t_token **tokens)
{
	int	i;

	i = 0;
	if (tokens == NULL)
		return ;
	while (tokens[i] != NULL)
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
