/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 11:42:52 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/04 10:29:54 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Initializes the environment variables from the provided array.
 *
 * This function creates a linked list of environment variables from the
 * `environ` array, which is typically provided by the operating system.
 * Each variable is split into a key and value at the '=' character.
 * @param environ The array of environment variables in the format "KEY=VALUE".
 * @return Returns a pointer to the head of the linked list of environment
 *         variables.
 */
char *get_env_value(t_env_var *env_list, char *key)
{
    t_env_var *current;
	
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
 * @brief Prints the environment variables to standard output.
 *
 * This function iterates through the linked list of environment variables
 * and prints each key-value pair in the format "KEY=VALUE".
 * @param env_list The head of the environment linked list.
 */
void builtin_env(t_env_var *env_list)
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
