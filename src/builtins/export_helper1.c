/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:25:01 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 12:36:52 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Updates an existing environment variable with a new value.
 * * This function handles the logic for appending or overwriting a variable's
 * value in the environment list.
 *
 * @param existing_var The existing environment variable node.
 * @param new_value The value to be stored.
 * @param append A flag indicating if this is an append operation.
 * @return 0 on success, 1 on failure.
 */
static int	update_existing_env_var(t_env_var *existing_var,
										char *new_value, int append)
{
	char	*joined;

	if (append && existing_var->value)
	{
		joined = ft_strjoin(existing_var->value, new_value);
		if (joined == NULL)
		{
			free(new_value);
			return (1);
		}
		free(existing_var->value);
		existing_var->value = joined;
		free(new_value);
	}
	else
	{
		if (existing_var->value != NULL)
			free(existing_var->value);
		existing_var->value = new_value;
	}
	return (0);
}

/**
 * @brief Stores or updates an environment variable based on parsed data.
 *
 * This function handles the logic for appending or overwriting a variable
 * in the environment list.
 *
 * @param key The variable key.
 * @param clean_value The value to be stored.
 * @param append A flag indicating if this is an append operation.
 * @param env_list The environment variable linked list.
 * @return 0 on success, 1 on failure.
 */
static int	store_export_assignment(char *key, char *clean_value,
										int append, t_env_var *env_list)
{
	t_env_var	*existing_var;

	existing_var = find_env_var(key, env_list);
	if (existing_var != NULL)
	{
		if (update_existing_env_var(existing_var, clean_value, append) == 1)
		{
			free(key);
			return (1);
		}
	}
	else
	{
		add_env_var(key, clean_value, env_list);
	}
	free(key);
	return (0);
}

/**
 * @brief Parses an export assignment to extract the key, value, and append flag.
 *
 * @param arg The full argument string (e.g., "VAR+=value").
 * @param key A pointer to store the extracted key.
 * @param clean_value A pointer to store the extracted value.
 * @param append A pointer to a flag for += operation.
 * @return 0 on success, 1 on failure.
 */
static int	parse_export_assignment(const char *arg, char **key,
										char **clean_value, int *append)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	*append = 0;
	if (equal_sign > arg && *(equal_sign - 1) == '+')
	{
		*append = 1;
		*key = ft_substr(arg, 0, (size_t)(equal_sign - arg - 1));
	}
	else
		*key = ft_substr(arg, 0, (size_t)(equal_sign - arg));
	if (*key == NULL || !is_valid_identifier(*key))
	{
		if (*key)
			free(*key);
		error_with_backticked_arg("export", arg, "not a valid identifier");
		return (1);
	}
	*clean_value = ft_strdup(equal_sign + 1);
	if (*clean_value == NULL)
	{
		free(*key);
		return (1);
	}
	return (0);
}

/**
 * @brief Handles the 'export NAME' case, where no value is assigned.
 *
 * This function validates the identifier and adds the variable to the
 * environment list with a NULL value if it doesn't already exist.
 *
 * @param arg The argument string (e.g., "MY_VAR").
 * @param env_list The environment variable linked list.
 * @return 0 on success, 1 on error.
 */
static int	handle_no_assignment_export(const char *arg, t_env_var *env_list)
{
	t_env_var	*existing_var;

	if (!is_valid_identifier((char *)arg))
	{
		error_with_backticked_arg("export", arg, "not a valid identifier");
		return (1);
	}
	existing_var = find_env_var((char *)arg, env_list);
	if (existing_var == NULL)
		add_env_var(ft_strdup(arg), NULL, env_list);
	return (0);
}

/**
 * @brief The main function to handle the export builtin.
 *
 * It parses the argument and calls appropriate helper functions to handle
 * assignment and non-assignment cases.
 *
 * @param arg The argument string from the command line.
 * @param env_list The environment variable linked list.
 * @return 0 on success, 1 on error.
 */
int	export_variable(const char *arg, t_env_var *env_list)
{
	char	*equal_sign;
	char	*key;
	char	*clean;
	int		append;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign == NULL)
		return (handle_no_assignment_export(arg, env_list));
	if (parse_export_assignment(arg, &key, &clean, &append) == 1)
		return (1);
	return (store_export_assignment(key, clean, append, env_list));
}
