/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_helper_2.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/05 10:39:19 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/19 08:51:51 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	handle_special_var(const char *input, size_t start,
								char **result, t_expand_data *data)
{
	char	*var_name;
	char	*value;
	char	*tmp;

	var_name = ft_strndup(&input[start], 1);
	value = get_var_value(var_name, data->env_list, data->last_exit_status);
	tmp = ft_strjoin(*result, value);
	free(*result);
	*result = tmp;
	free(var_name);
	free(value);
	return (start + 1);
}

int	expand_simple_variable(const char *input, size_t i,
								char **result, t_expand_data *data)
{
	size_t	start;

	start = i + 1;
	if (input[start] == '?' || input[start] == '$' || input[start] == '0')
		return (handle_special_var(input, start, result, data));
	else if ((input[start] >= 'A' && input[start] <= 'Z')
		|| (input[start] >= 'a' && input[start] <= 'z') || input[start] == '_')
		return (handle_standard_var(input, start, result, data));
	else
		return (handle_default_case(input, i, result));
}

static char	*get_special_var_value(const char *var_name, int last_exit_status)
{
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(last_exit_status));
	// if (ft_strncmp(var_name, "$", 2) == 0)
	// 	return (ft_itoa(getpid())); // not required by subject, also getpid() function not allowed 
	if (ft_strncmp(var_name, "0", 2) == 0)
		return (ft_strdup("minishell"));
	return (NULL);
}

char	*get_var_value(const char *var_name,
								t_env_var *env_list, int last_exit_status)
{
	t_env_var	*env_var;
	char		*value;

	value = get_special_var_value(var_name, last_exit_status);
	if (value != NULL)
		return (value);
	env_var = find_env_var(var_name, env_list);
	if (env_var != NULL)
		return (ft_strdup(env_var->value));
	return (ft_strdup(""));
}

int	expand_braced_variable(const char *input, size_t i,
								char **result, t_expand_data *data)
{
	size_t	start;
	size_t	end;
	char	*var_name;
	char	*value;
	char	*tmp;

	start = i + 2;
	end = start;
	while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
		end++;
	if (input[end] == '}')
	{
		var_name = ft_strndup(&input[start], end - start);
		value = get_var_value(var_name, data->env_list, data->last_exit_status);
		tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(var_name);
		free(value);
		return (end + 1);
	}
	return (i);
}
