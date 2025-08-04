/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 11:42:52 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/04 08:42:16 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// /**
//  * @brief Implements the 'env' built-in command.
//  *
//  * Prints all environment variables. If any arguments are provided,
//  * it prints an error message and returns a failure status, mimicking bash.
//  * @param args The arguments array. Should be empty other than "env".
//  * @param envp The environment variables array.
//  * @return Returns 0 on success, 127 on failure (if arguments are given).
//  */
// int builtin_env(char **args, char **envp)
// {
// 	int i;

// 	// If there are arguments, print an error message
// 	if (args[1] != NULL)
// 	{
// 		ft_putstr_fd("env: ‘", STDERR_FILENO);
// 		ft_putstr_fd(args[1], STDERR_FILENO);
// 		ft_putstr_fd("’: No such file or directory\n", STDERR_FILENO);
// 		return (127); // Mimics bash's error for `env foo`
// 	}

// 	i = 0;
// 	while (envp && envp[i])
// 	{
// 		ft_putstr_fd(envp[i], STDOUT_FILENO);
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		i++;
// 	}
// 	return (0);
// }

// char *get_env_value(t_env_var *env, char *key)
// {
//     while (env)
//     {
//         if (strcmp(env->key, key) == 0)
//             return env->value;
//         env = env->next;
//     }
//     return NULL;
// }

char *get_env_value(t_env_var *env_list, char *key)
{
    t_env_var *current = env_list;
    while (current)
    {
        if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
            return current->value;
        current = current->next;
    }
    // PATH not found, debug print here
    // fprintf(stderr, "Debug: %s variable not found in env_list\n", key);
    return NULL;
}

// TO DO: NEEDS MORE ROBUST ERROR HANDLING
void builtin_env(t_env_var *env_list)
{
    while (env_list)
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
