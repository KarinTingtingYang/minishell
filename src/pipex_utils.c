/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:08:55 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/22 12:59:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// static int	wait_for_children(pid_t first_pid, pid_t last_pid)
// {
// 	int	status;

// 	waitpid(first_pid, NULL, 0);
// 	waitpid(last_pid, &status, 0);
// 	if (WIFSIGNALED(status))
// 		return (128 + WTERMSIG(status));
// 	if (WIFEXITED(status))
// 		return (WEXITSTATUS(status));
// 	return (1);
// }

// static void	run_first_child(int pipe_fd[2], char *cmd_line, char **envp)
// {
// 	char **cmd = ft_split(cmd_line, ' ');
// 	char **path_dirs = find_path_dirs(envp);
// 	char *cmd_path = find_full_cmd_path(cmd[0], path_dirs);

// 	dup2(pipe_fd[1], STDOUT_FILENO);
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);

// 	execute_cmd(cmd_path, cmd, path_dirs);
// }

// static void	run_last_child(int pipe_fd[2], char *cmd_line, char **envp)
// {
// 	char **cmd = ft_split(cmd_line, ' ');
// 	char **path_dirs = find_path_dirs(envp);
// 	char *cmd_path = find_full_cmd_path(cmd[0], path_dirs);

// 	dup2(pipe_fd[0], STDIN_FILENO);
// 	close(pipe_fd[1]);
// 	close(pipe_fd[0]);

// 	execute_cmd(cmd_path, cmd, path_dirs);
// }

// static int	last_child(int pipe_fd[2], char *cmd2_line, char **envp)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid < 0)
// 		return (pid);
// 	if (pid == 0)
// 		run_last_child(pipe_fd, cmd2_line, envp);
// 	return (pid);
// }

// static int	first_child(int pipe_fd[2], char *cmd1_line, char **envp)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid < 0)
// 		return (pid);
// 	if (pid == 0)
// 		run_first_child(pipe_fd, cmd1_line, envp);
// 	return (pid);
// }

// int	run_pipex(char *cmd1_line, char *cmd2_line, char **envp)
// {
// 	int pipe_fd[2];
// 	pid_t first_pid;
// 	pid_t last_pid;

// 	if (pipe(pipe_fd) == -1)
// 		error_exit("pipe");
// 	first_pid = first_child(pipe_fd, cmd1_line, envp);
// 	if (first_pid < 0)
// 		error_exit("fork first");
// 	last_pid =  last_child(pipe_fd, cmd2_line, envp);
// 	if (last_pid < 0)
// 	{
// 		waitpid(first_pid, NULL, 0);
// 		error_exit("fork second");
// 	}
// 	close(pipe_fd[0]);
// 	close(pipe_fd[1]);
// 	return (wait_for_children(first_pid, last_pid));
// }



// pipex_utils.c

// 1. Create pipes recursively
static int create_pipe_recursive(int **pipes, int idx, int max)
{
    if (idx >= max)
        return 0;

    pipes[idx] = malloc(sizeof(int) * 2);
    if (!pipes[idx])
        return -1;

    if (pipe(pipes[idx]) == -1)
    {
        perror("pipe");
        return -1;
    }

    if (create_pipe_recursive(pipes, idx + 1, max) == -1)
    {
        free(pipes[idx]);
        return -1;
    }
    return 0;
}

static int **create_pipes(int cmd_count)
{
    int **pipes;

    if (cmd_count < 2)
        return NULL;

    pipes = malloc(sizeof(int *) * (cmd_count - 1));
    if (!pipes)
        return NULL;

    if (create_pipe_recursive(pipes, 0, cmd_count - 1) == -1)
    {
        int i = 0;
        while (i < cmd_count - 1)
        {
            if (pipes[i])
                free(pipes[i]);
            i++;
        }
        free(pipes);
        return NULL;
    }
    return pipes;
}

// 2. Close pipes recursively
static void close_pipes_recursive(int **pipes, int idx, int max)
{
    if (idx >= max)
        return;
    close(pipes[idx][0]);
    close(pipes[idx][1]);
    close_pipes_recursive(pipes, idx + 1, max);
}

static void close_pipes(int **pipes, int count)
{
    close_pipes_recursive(pipes, 0, count);
}

static void free_pipes_recursive(int **pipes, int idx, int max)
{
    if (idx >= max)
        return;
    free(pipes[idx]);
    free_pipes_recursive(pipes, idx + 1, max);
}

static void free_pipes(int **pipes, int count)
{
    free_pipes_recursive(pipes, 0, count);
    free(pipes);
}

// 3. Wait for all children recursively
static int wait_all_recursive(pid_t *pids, int idx, int max, int last_status)
{
    int status;

    if (idx >= max)
        return last_status;

    if (waitpid(pids[idx], &status, 0) == -1)
    {
        perror("waitpid");
        return wait_all_recursive(pids, idx + 1, max, -1);
    }

    if (WIFEXITED(status))
    {
        if (idx == max - 1)
            last_status = WEXITSTATUS(status);
    }
    else if (WIFSIGNALED(status))
    {
        if (idx == max - 1)
            last_status = 128 + WTERMSIG(status);
    }

    return wait_all_recursive(pids, idx + 1, max, last_status);
}

static int wait_all_children(pid_t *pids, int count)
{
    return wait_all_recursive(pids, 0, count, 0);
}

// 4. Fork and execute pipeline recursively
static int fork_recursive(t_command **cmds, int **pipes, char **path_dirs, pid_t *pids, int i, int cmd_count)
{
    if (i >= cmd_count)
        return 0;

    pids[i] = fork();
    if (pids[i] == -1)
    {
        perror("fork");
        return -1;
    }

    if (pids[i] == 0)
    {
        reset_child_signal_handlers();

        if (i > 0)
            dup2(pipes[i - 1][0], STDIN_FILENO);

        if (i < cmd_count - 1)
            dup2(pipes[i][1], STDOUT_FILENO);

        close_pipes(pipes, cmd_count - 1);

        execute_cmd(cmds[i]->cmd_path, cmds[i]->args, path_dirs);

        perror("execve");
        exit(EXIT_FAILURE);
    }
    return fork_recursive(cmds, pipes, path_dirs, pids, i + 1, cmd_count);
}

// 5. Main execute pipeline function (exposed)
int execute_pipeline(t_command **cmds, int cmd_count, char **path_dirs)
{
    int **pipes = NULL;
    pid_t *pids = NULL;
    int res;

    if (cmd_count < 1)
        return 0;

    if (cmd_count > 1)
    {
        pipes = create_pipes(cmd_count);
        if (!pipes)
            return -1;
    }

    pids = malloc(sizeof(pid_t) * cmd_count);
    if (!pids)
    {
        if (pipes)
        {
            close_pipes(pipes, cmd_count - 1);
			free_pipes(pipes, cmd_count - 1);
        }
        return -1;
    }

    res = fork_recursive(cmds, pipes, path_dirs, pids, 0, cmd_count);
    if (res == -1)
    {
        free(pids);
        if (pipes)
        {
            close_pipes(pipes, cmd_count - 1);
            int i = 0;
            while (i < cmd_count - 1)
            {
                free(pipes[i]);
                i++;
            }
            free(pipes);
        }
        return -1;
    }

    if (pipes)
    {
        close_pipes(pipes, cmd_count - 1);
        int i = 0;
        while (i < cmd_count - 1)
        {
            free(pipes[i]);
            i++;
        }
        free(pipes);
    }

    int exit_status = wait_all_children(pids, cmd_count);
    free(pids);
    return exit_status;
}
