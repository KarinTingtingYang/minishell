/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:41:01 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles special variable cases like $?, $$, and $0.
 * 
 * This function processes special variables and appends their values
 * to the result string.
 * @param input The original input string.
 * @param start The starting index of the variable name in the input.
 * @param result Pointer to the result string being built.
 * @param data The expansion data containing environment variables and
 * last exit status.
 * @return The index in the input string after processing the variable.
 */
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

/**
 * @brief Expands a simple variable (e.g., $VAR) in the input string.
 * 
 * This function identifies the type of variable and delegates to the
 * appropriate handler function to perform the expansion.
 * @param input The original input string.
 * @param i The index of the '$' character in the input.
 * @param result Pointer to the result string being built.
 * @param data The expansion data containing environment variables and
 * last exit status.
 * @return The index in the input string after processing the variable.
 */
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

/**
 * @brief Retrieves the value of a special variable like $?, $$, or $0.
 * 
 * This function checks for special variable names and returns their
 * corresponding values. If the variable is not special, it returns NULL.
 * @param var_name The name of the variable (e.g., "?", "$", "0").
 * @param last_exit_status The last exit status to use for $?.
 * @return A newly allocated string with the variable's value,
 * or NULL if the variable is not special.
 */
static char	*get_special_var_value(const char *var_name, int last_exit_status)
{
	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(last_exit_status));
	if (ft_strncmp(var_name, "0", 2) == 0)
		return (ft_strdup("minishell"));
	return (NULL);
}

/**
 * @brief Retrieves the value of an environment variable.
 * 
 * This function first checks for special variables like $?, $$, and $0.
 * If not special, it looks up the variable in the provided environment
 * list. If found, it returns its value; otherwise, it returns an empty
 * string.
 * @param var_name The name of the variable to look up.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status for $?.
 * @return A newly allocated string with the variable's value,
 * or an empty string if not found.
 */
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

/**
 * @brief Expands a braced variable (e.g., ${VAR}) in the input string.
 * 
 * This function extracts the variable name from within braces, retrieves
 * its value, and appends it to the result string.
 * @param input The original input string.
 * @param i The index of the '$' character in the input.
 * @param result Pointer to the result string being built.
 * @param data The expansion data containing environment variables and
 * last exit status.
 * @return The index in the input string after processing the variable.
 */
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
