/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:47:50 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 11:50:42 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Removes an environment variable from the linked list by its key.
 *
 * This function searches for the environment variable with the specified key
 * in the linked list and removes it if found. It properly frees the memory
 * associated with the variable.
 *
 * @param env_list Pointer to the head of the environment linked list.
 * @param key The key of the environment variable to remove.
 */
static void	remove_env_var(t_env_var **env_list, const char *key)
{
	t_env_var	*current_var;
	t_env_var	*previous_var;

	current_var = *env_list;
	previous_var = NULL;
	while (current_var != NULL)
	{
		if (ft_strncmp(current_var->key, key, ft_strlen(key) + 1) == 0)
		{
			if (previous_var != NULL)
				previous_var->next = current_var->next;
			else
				*env_list = current_var->next;
			free(current_var->key);
			free(current_var->value);
			free(current_var);
			return ;
		}
		previous_var = current_var;
		current_var = current_var->next;
	}
}

/**
 * @brief Checks if a variable name is valid.
 * A valid variable name starts with a letter or underscore,
 * followed by letters, digits, or underscores.
 *
 * @param str The variable name to check.
 * @return 1 if valid, 0 otherwise.
 */
static int	is_valid_varname(const char *str)
{
	int	i;

	i = 0;
	if (str == NULL || !(ft_isalpha(str[0]) || str[0] == '_'))
		return (0);
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Implements the 'unset' built-in command.
 * Removes environment variables from the linked list.
 *
 * @param env_list Pointer to the head of the environment linked list.
 * @param args The command arguments. args[0] is "unset", args[1..n] are the
 * variable names to unset.
 * @return Returns 0 on success, 1 on failure (invalid variable name).
 */
int	run_unset(t_env_var **env_list, char **args)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	while (args[i] != NULL)
	{
		if (ft_strchr(args[i], '=') != NULL)
		{
			i++;
			continue ;
		}
		if (!is_valid_varname(args[i]))
		{
			i++;
			continue ;
		}
		remove_env_var(env_list, args[i]);
		i++;
	}
	return (exit_status);
}
