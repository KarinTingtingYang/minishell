/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 10:45:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../src/includes/pipex.h"

static void	handle_execve_error(char *cmd_path, char **args, char **path_dirs)
{
	int	exit_status;

	if (errno == ENOENT)
	{
		write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
		write(STDERR_FILENO, " : command not found\n", 22);
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

static void	execute_cmd(char *cmd_p, char **args, char **path_dirs, char **envp)
{
	if (cmd_p == NULL)
	{
		free_split(path_dirs);
		free_split(args);
		error_msg_exit(args[0]);
	}
	execve(cmd_p, args, envp);
	handle_execve_error(cmd_p, args, path_dirs);
}

static char	**find_path_dirs(char **envp)
{
	int		i;
	char	*path_value;

	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (envp[i] == NULL)
		return (NULL);
	path_value = envp[i] + 5;
	return (ft_split(path_value, ':'));
}

void	run_last_child(int pipe_fd[2], char **argv, char **envp)
{
	int		output_fd;
	char	**args;
	char	**path_dirs;
	char	*cmd_path;

	output_fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (output_fd < 0)
		error_exit(argv[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	dup2(output_fd, STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(output_fd);
	args = ft_split(argv[0], ' ');
	if (args == NULL || args[0] == NULL)
		error_msg_exit("command parse error");
	path_dirs = find_path_dirs(envp);
	cmd_path = find_full_cmd_path(args[0], path_dirs);
	execute_cmd(cmd_path, args, path_dirs, envp);
}

void	run_first_child(int pipe_fd[2], char **argv, char **envp)
{
	int		input_fd;
	char	**args;
	char	**path_dirs;
	char	*cmd_path;

	input_fd = open(argv[0], O_RDONLY);
	if (input_fd < 0)
		error_exit(argv[0]);
	dup2(input_fd, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	close(input_fd);
	args = ft_split(argv[1], ' ');
	if (args == NULL || args[0] == NULL)
		error_msg_exit("command parse error");
	path_dirs = find_path_dirs(envp);
	cmd_path = find_full_cmd_path(args[0], path_dirs);
	execute_cmd(cmd_path, args, path_dirs, envp);
}
