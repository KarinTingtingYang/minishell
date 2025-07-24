/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 10:20:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 13:03:20 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Closes and frees all pipe file descriptors recursively.
 *
 * This function closes both ends of each pipe and frees the memory allocated
 * for each pipe. It uses recursion to clean up from the given index up to `max`.
 * Once all pipes are closed and freed, the top-level `pipes` array is also freed.
 * @param pipes  A pointer to an array of pipe file descriptors.
 * @param idx    The current index of the pipe to close and free.
 * @param max    The total number of pipes in the array.
 */
void close_free_pipes_recursively(int **pipes, int idx, int max)
{
    if (pipes == NULL || idx >= max)
        return ;
    close(pipes[idx][0]);
    close(pipes[idx][1]);
    free(pipes[idx]);
    close_free_pipes_recursively(pipes, idx + 1, max);
    if (idx == 0)
        free(pipes);
}

/**
 * @brief Recursively allocates and creates pipes between commands.
 *
 * This function initializes each pipe for inter-process communication
 * between commands in a pipeline. It uses recursion to create all required
 * pipes from the current index up to the maximum.
 * On failure (either allocation or pipe creation), it returns -1
 * and stops recursion. The caller is responsible for freeing any successfully
 * allocated pipes up to failure.
 * @param pipes  A pointer to an array of pipe file descriptors.
 * @param index  The current pipe index being created.
 * @param max    The total number of pipes to create.
 *
 * @return 0 on success, -1 on failure.
 */
int create_all_pipes_recursively(int **pipes, int index, int max)
{
    if (index >= max)
        return (0);
    pipes[index] = malloc(sizeof(int) * 2);
    if (pipes[index] == NULL)
        return (-1);
    if (pipe(pipes[index]) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (create_all_pipes_recursively(pipes, index + 1, max) == -1)
    {
        free(pipes[index]);
        return (-1);
    }
    return (0);
}

/**
 * @brief Allocates memory for storing child process IDs (PIDs).
 *
 * This function allocates an array of `pid_t` to store the PIDs of
 * child processes that will be created later in the pipeline. If allocation
 * fails, it also handles cleanup by freeing any already-created pipes.
 * @param cmd_count  The number of commands (and thus child processes) expected.
 * @param pipes      A 2D array of pipe file descriptors to be freed on failure.
 * @return A pointer to the allocated `pid_t` array on success,
 *         or NULL if memory allocation fails.
 */
pid_t *create_child_processes(int cmd_count, int **pipes)
{
    pid_t *child_processes;

    child_processes = malloc(sizeof(pid_t) * cmd_count);
    if (child_processes == NULL)
    {
        close_free_pipes_recursively(pipes, 0, cmd_count - 1);
        return (NULL);
    }
    return (child_processes);
}

/**
 * @brief Allocates and initializes pipe file descriptors for inter-process
 * communication.
 *
 * This function prepares an array of pipes to connect commands in a pipeline,
 * by allocating memory and initializing each pipe using
 * `create_pipe_fds_between_commands`. It is only useful when `cmd_count`
 * is greater than 1 (for actual piping).
 * @param cmd_count  Number of commands in the pipeline.
 * @return A pointer to a 2D array of pipe file descriptors on success,
 *         or NULL if allocation or pipe creation fails.
 */
int **prepare_pipe_fds(int cmd_count)
{
    int **pipes;
	pipes = create_pipe_fds_between_commands(cmd_count);
    if (pipes == NULL)
        return (NULL);
    return (pipes);
}
