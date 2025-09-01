/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:42:52 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 13:50:05 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
