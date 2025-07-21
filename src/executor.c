/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/04 13:55:56 by makhudon      #+#    #+#                 */
/*   Updated: 2025/07/21 14:18:52 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

extern char **environ;
extern volatile sig_atomic_t g_child_running; // Declare extern for global flag

static void handle_execve_error(char *cmd_path, char **args, char **path_dirs)
{
	int exit_status;

	if (errno == ENOENT)
	{
		write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit_status = 127;
	}
	else
	{
		perror(cmd_path);
		exit_status = 1;
	}
	free_split(path_dirs);
	free_split(args);
	free(cmd_path);
	exit(exit_status);
}

static void execute_cmd(char *cmd_path, char **args, char **path_dirs)
{
	
    // Reset signal handlers to default for the child process
    reset_child_signal_handlers(); //
	if (cmd_path == NULL)
	{
		free_split(path_dirs);
		free_split(args);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	execve(cmd_path, args, environ);
	handle_execve_error(cmd_path, args, path_dirs);
}

int execute_command(char *line, char **envp)
{
	(void)envp; // envp is not used in this function, but can be used for future enhancements
	char	**args;
	char	**path_dirs;
	char	*cmd_path;
	pid_t	pid;
	// int		status; --> moved to helper function wait_for_child_and_handle_status() in signal.c
	int		exit_code; // new variable to hold exit code returned from helper function 
					   // wait_for_child_and_handle_status()

	args = ft_split(line, ' ');
	path_dirs = find_path_dirs(environ);
	cmd_path = find_full_cmd_path(args[0], path_dirs);
	pid = fork();
	if (line == NULL || *line == '\0')
		return 0;
	if (args == NULL || args[0] == NULL) 
	{
		free_split(args);
		return (0);
	}
	if (pid < 0)
	{
		perror("fork");
		free_split(path_dirs);
		free_split(args);
		free(cmd_path);
		return (-1);
	}
	else if (pid == 0)
	{
		execute_cmd(cmd_path, args, path_dirs);
	}
	else
	{
		// moved wpid and exit code check to helper function wait_for_child_and_handle_status()
		exit_code = wait_for_child_and_handle_status(pid);
		free_split(path_dirs);
		free_split(args);
		free(cmd_path);
		return (exit_code);
	}
	return 0;
}
