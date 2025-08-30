/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:42:52 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 11:35:28 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Retrieves the value of an environment variable by its key.
 *
 * This function searches the linked list of environment variables for a
 * variable with the specified key and returns its value.
 * @param env_list The head of the environment linked list.
 * @param key The key of the environment variable to retrieve.
 * @return The value of the environment variable, or NULL if not found.
 */
char	*get_env_value(t_env_var *env_list, char *key)
{
	t_env_var	*current;

	current = env_list;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/**
 * @brief Implements the 'env' built-in command.
 * Prints all environment variables in the format KEY=VALUE.
 * Only variables with a value are printed.
 *
 * @param env_list The linked list of environment variables.
 */
void	run_env(t_env_var *env_list)
{
	while (env_list != NULL)
	{
		if (env_list->value)
		{
			ft_putstr_fd(env_list->key, STDOUT_FILENO);
			ft_putstr_fd("=", STDOUT_FILENO);
			ft_putstr_fd(env_list->value, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env_list = env_list->next;
	}
}
