/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:08:55 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/22 11:12:28 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

static int	wait_for_children(pid_t first_pid, pid_t last_pid)
{
	int	status;

	waitpid(first_pid, NULL, 0);
	waitpid(last_pid, &status, 0);
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static void	run_first_child(int pipe_fd[2], char *cmd_line, char **envp)
{
	char **cmd = ft_split(cmd_line, ' ');
	char **path_dirs = find_path_dirs(envp);
	char *cmd_path = find_full_cmd_path(cmd[0], path_dirs);

	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	execute_cmd(cmd_path, cmd, path_dirs);
}

static void	run_last_child(int pipe_fd[2], char *cmd_line, char **envp)
{
	char **cmd = ft_split(cmd_line, ' ');
	char **path_dirs = find_path_dirs(envp);
	char *cmd_path = find_full_cmd_path(cmd[0], path_dirs);

	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[1]);
	close(pipe_fd[0]);

	execute_cmd(cmd_path, cmd, path_dirs);
}

static int	last_child(int pipe_fd[2], char *cmd2_line, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (pid);
	if (pid == 0)
		run_last_child(pipe_fd, cmd2_line, envp);
	return (pid);
}

static int	first_child(int pipe_fd[2], char *cmd1_line, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (pid);
	if (pid == 0)
		run_first_child(pipe_fd, cmd1_line, envp);
	return (pid);
}

int	run_pipex(char *cmd1_line, char *cmd2_line, char **envp)
{
	int pipe_fd[2];
	pid_t first_pid;
	pid_t last_pid;

	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
	first_pid = first_child(pipe_fd, cmd1_line, envp);
	if (first_pid < 0)
		error_exit("fork first");
	last_pid =  last_child(pipe_fd, cmd2_line, envp);
	if (last_pid < 0)
	{
		waitpid(first_pid, NULL, 0);
		error_exit("fork second");
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (wait_for_children(first_pid, last_pid));
}
