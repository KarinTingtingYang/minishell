/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 09:00:14 by makhudon      #+#    #+#                 */
/*   Updated: 2025/07/31 10:04:00 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// TO DO: THE RETURN VALUES OF BUILTINS ARE NOT CONSISTENT 
// (CD AND PWD RETURN 0 ON SUCCESS, ECHO BUILTIN RETURNS NOTHING)


// returns 0 on success, 1 on failure
int run_builtin(char **args, t_env_var *env_list)
{
	if (args == NULL || args[0] == NULL)
		return (1);
	if (ft_strncmp(args[0], "echo", ft_strlen("echo") + 1) == 0)
	{
		run_echo(args);
		return (0);
	}
	else if (ft_strncmp(args[0], "cd", ft_strlen("cd") + 1) == 0)
	{
		return (builtin_cd(args));
	}
	else if (ft_strncmp(args[0], "pwd", ft_strlen("pwd") + 1) == 0)
	{
		return (builtin_pwd());
	}
	else if (ft_strncmp(args[0], "exit", ft_strlen("exit") + 1) == 0)
	{
		return (builtin_exit(args));
	}
	else if (ft_strncmp(args[0], "env", 4) == 0)
	{
		builtin_env(env_list);
		return (0);
	}
	else if (ft_strncmp(args[0], "export", 7) == 0)
    {
        return (run_export(env_list, args)); // Pass the list to export
    }
	return (0); // should not reach here if is_builtin is used correctly
}

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	return (ft_strncmp(cmd, "echo", 5) == 0 ||
			ft_strncmp(cmd, "cd", 3) == 0 || 
			ft_strncmp(cmd, "pwd", 4) == 0 ||
			ft_strncmp(cmd, "exit", 5) == 0 ||
			ft_strncmp(cmd, "env", 4) == 0 ||
			ft_strncmp(cmd, "export", 7) == 0);
	// Add more built-ins as needed
}