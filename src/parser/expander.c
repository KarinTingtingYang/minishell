/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/04 13:45:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Finds the value of an environment variable by its name.
 * 
 * This function searches for the given variable name in the linked list
 * of environment variables. If found, it returns a dynamically allocated
 * string containing the variable's value. If the variable is not found,
 * it returns an empty string.
 * @param var_name The name of the environment variable to find.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A dynamically allocated string with the variable's value, or
 *         an empty string if not found.
 */
static char	*get_variable_value(const char *var_name, t_env_var *env_list, int last_exit_status)
{
	t_env_var *env_var;

	if (ft_strncmp(var_name, "?", 2) == 0)
		return (ft_itoa(last_exit_status));
	if (ft_strncmp(var_name, "$", 2) == 0)
		return (ft_itoa(getpid()));
	if (ft_strncmp(var_name, "0", 2) == 0)
        return (ft_strdup("minishell"));
	env_var = find_env_var(var_name, env_list);
	if (env_var != NULL)
		return (ft_strdup(env_var->value));
	return (ft_strdup(""));
}

/**
 * @brief Expands variables in a string based on the environment variables.
 * 
 * This function processes the input string, replacing variable references
 * (e.g., $VAR) with their corresponding values from the environment list.
 * It handles single and double quotes to determine when to expand variables.
 * @param input The input string containing variable references.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A new string with variables expanded, or NULL on failure.
 */
char *expand_variables(char *input, t_env_var *env_list, int last_exit_status)
{
	char	*result;
	size_t	i;
	int		inside_single_quote;
	int		inside_double_quote;

	result = ft_strdup("");
	i = 0;
	inside_single_quote = 0;
	inside_double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && inside_double_quote == 0)
			inside_single_quote = !inside_single_quote;
		else if (input[i] == '"' && inside_single_quote == 0)
			inside_double_quote = !inside_double_quote;
		else if (input[i] == '$' && inside_single_quote == 0 && input[i + 1] != '\0')
		{
			size_t start = i + 1;

			if (input[start] == '?' || input[start] == '$' || input[start] == '0')
				i = start + 1;
			else
			{
				i = start;
				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
					i++;
			}
			char *var_name = ft_strndup(&input[start], i - start);
			char *value = get_variable_value(var_name, env_list, last_exit_status);
			char *tmp = ft_strjoin(result, value);
			free(result);
			result = tmp;
			free(var_name);
			free(value);
			continue;
		}
		else
			result = append_char(result, input[i]);
		i++;
	}
	return (result);
}

/**
 * @brief Expands all arguments in a command by replacing variables with their values.
 * 
 * This function iterates through an array of command arguments, expanding each
 * argument using the `expand_variables` function. It modifies the original array
 * by freeing the old argument and replacing it with the expanded version.
 * @param args The array of command arguments to expand.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 */
void expand_args(char **args, t_env_var *env_list, int last_exit_status)
{
    int i = 0;
    char *expanded;

    while (args[i])
    {
        expanded = expand_variables(args[i], env_list, last_exit_status);
        free(args[i]);
        args[i] = expanded;
        i++;
    }
}
