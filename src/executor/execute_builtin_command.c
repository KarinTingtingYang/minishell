/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 11:55:08 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/05 12:07:00 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks for and handles errors from the `dup` system call.
 *
 * This function verifies if `dup` failed to duplicate standard input or
 * output file descriptors. If an error is detected (indicated by a return
 * value of -1), it cleans up any file descriptors that were successfully
 * duplicated before the error occurred. It then reports the error using
 * `ft_error_and_exit` and terminates the program with a failure status.
 *
 * @param saved_stdin A pointer to the file descriptor for standard input.
 * @param saved_stdout A pointer to the file descriptor for standard output.
 */
static void	check_and_handle_dup_error(int *saved_stdin, int *saved_stdout)
{
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		ft_error_and_exit("dup", strerror(errno), EXIT_FAILURE);
	}
}

/**
 * @brief Prepares and manages input/output redirection for builtin commands.
 *
 * This function handles the redirection of standard input and output for
 * a builtin command. It first checks if any redirection is required based on
 * the `data` provided. If so, it duplicates the current file descriptors for
 * standard input and output to allow them to be restored later. It then
 * calls a helper function to apply the specified redirections, handling
 * potential errors that may arise during file descriptor duplication or
 * redirection application.
 *
 * @param data A pointer to the structure containing the paths for input and
 * output files.
 * @param saved_stdin A pointer to an integer where the original standard input
 * file descriptor will be stored.
 * @param saved_stdout A pointer to an integer where the original standard
 * output file descriptor will be stored.
 * @param did_save A pointer to a flag that will be set to 1 if the original
 * file descriptors were successfully saved.
 * @return Returns 0 on successful setup of redirection, or 1 if an error
 * occurred during the redirection process.
 */
static int	setup_builtin_io(t_execute_data *data, int *saved_stdin,
									int *saved_stdout, int *did_save)
{
	int	need_save;

	*saved_stdin = -1;
	*saved_stdout = -1;
	*did_save = 0;
	need_save = 0;
	if (data->input_file != NULL || data->output_file != NULL)
		need_save = 1;
	if (need_save)
	{
		*saved_stdin = dup(STDIN_FILENO);
		*saved_stdout = dup(STDOUT_FILENO);
		check_and_handle_dup_error(saved_stdin, saved_stdout);
		*did_save = 1;
	}
	if (apply_builtin_redirection(data->input_file, data->output_file,
			data->output_mode) == -1)
		return (1);
	return (0);
}

/**
 * @brief Restores standard input and output file descriptors
 *        after redirection.
 *
 * This function reverses the effects of I/O redirection
 * for a builtin command by restoring the original standard input
 * and output streams. It first checks if any file descriptors were
 * saved before attempting to restore. It uses `dup2` to duplicate
 * the saved file descriptors back to their original locations
 * (`STDIN_FILENO` and `STDOUT_FILENO`), handling potential
 * errors during the process. Finally, it closes the saved file
 * descriptors to prevent resource leaks.
 *
 * @param did_save A flag indicating whether the original
 *                 file descriptors were saved.
 * @param saved_stdin The saved file descriptor for standard input.
 * @param saved_stdout The saved file descriptor for standard output.
 */
static void	restore_builtin_io(int did_save, int saved_stdin, int saved_stdout)
{
	if (!did_save)
		return ;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
	close(saved_stdin);
	close(saved_stdout);
}

/**
 * @brief Executes a builtin command with proper redirection handling.
 *
 * This function saves the original stdin and stdout file descriptors,
 * applies the necessary redirections for the builtin command, executes
 * the command, and then restores the original file descriptors.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @return The exit status of the executed builtin command.
 */
int	execute_builtin_command(t_execute_data *data, t_process_data *process_data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	did_save;
	int	status;

	status = setup_builtin_io(data, &saved_stdin, &saved_stdout, &did_save);
	if (status == 0)
		status = run_builtin(data->clean_args, process_data);
	restore_builtin_io(did_save, saved_stdin, saved_stdout);
	return (status);
}
