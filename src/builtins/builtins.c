/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:00:14 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/18 09:55:18 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// TO DO: THE RETURN VALUES OF BUILTINS ARE NOT CONSISTENT 
// (CD AND PWD RETURN 0 ON SUCCESS, ECHO BUILTIN RETURNS NOTHING)

/**
 * @brief Executes the built-in commands based on the provided arguments.
 *
 * This function checks the first argument to determine which built-in command
 * to execute and calls the corresponding function.
 * @param args The arguments array, where args[0] is the command name.
 * @param env_list The environment variable linked list.
 * @return Returns 0 on success, or an error code on failure.
 */
int run_builtin(char **args, t_env_var *env_list)
{
	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_strncmp(args[0], "echo", 5) == 0)
		// return (run_echo(args), 0);
		return (run_echo(args, env_list), 0);
	if (ft_strncmp(args[0], "cd", 3) == 0)
		return (run_cd(args, env_list));
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		return (run_pwd());
	if (ft_strncmp(args[0], "exit", 5) == 0)
		return (run_exit(args));
	if (ft_strncmp(args[0], "env", 4) == 0)
		return (run_env(env_list), 0);
	if (ft_strncmp(args[0], "export", 7) == 0)
		return (run_export(env_list, args));
	if (ft_strncmp(args[0], "unset", 6) == 0)
		return (run_unset(&env_list, args));
	return (0);
}

/**
 * @brief Checks if a command is a built-in command.
 *
 * @param cmd The command to check.
 * @return 1 if it is a built-in command, 0 otherwise.
 */
int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (ft_strncmp(cmd, "echo", 5) == 0 ||
			ft_strncmp(cmd, "cd", 3) == 0 || 
			ft_strncmp(cmd, "pwd", 4) == 0 ||
			ft_strncmp(cmd, "exit", 5) == 0 ||
			ft_strncmp(cmd, "env", 4) == 0 ||
			ft_strncmp(cmd, "export", 7) == 0 ||
			ft_strncmp(cmd, "unset", 6) == 0);
}
