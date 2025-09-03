/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/21 12:41:05 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 14:20:45 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Sets up signal handlers for the minishell (parent process).
 *
 * Configures SIGINT (Ctrl+C) to use `handle_parent_sigint` and
 * SIGQUIT (Ctrl+\) to be ignored.
 */
void setup_signal_handlers(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sa_int.sa_handler = handle_parent_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) == -1)
	{
		ft_error_and_exit("sigaction SIGINT", strerror(errno), EXIT_FAILURE);
	}
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) == -1)
	{
		ft_error_and_exit("sigaction SIGQUIT", strerror(errno), EXIT_FAILURE);
	}
}

/**
 * @brief Resets signal handlers to default behavior for child processes.
 *
 * This function should be called in the child process *after* fork() and
 * *before* execve(). This ensures that the executed command (e.g., `ls`)
 * handles signals with its default behavior, allowing SIGINT to terminate it.
 */
void reset_child_signal_handlers(void)
{
	struct sigaction sa_int_dfl;
	struct sigaction sa_quit_dfl;

	sa_int_dfl.sa_handler = SIG_DFL;
	sigemptyset(&sa_int_dfl.sa_mask);
	sa_int_dfl.sa_flags = 0;
	if (sigaction(SIGINT, &sa_int_dfl, NULL) == -1)
	{
		ft_error_and_exit("sigaction SIGINT child",
			strerror(errno), EXIT_FAILURE);
	}
	sa_quit_dfl.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit_dfl.sa_mask);
	sa_quit_dfl.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit_dfl, NULL) == -1)
	{
		ft_error_and_exit("sigaction SIGQUIT child",
			strerror(errno), EXIT_FAILURE);
	}
}

/*
 * @brief Waits for a specific child process to finish.
 *
 * This function wraps waitpid to wait for the given pid. It handles
 * interruptions by signals (EINTR) by retrying the wait.
 *
 * @param pid The process ID of the child to wait for.
 * @param status Pointer to an integer where the exit status will be stored.
 * @return The PID of the terminated child, or -1 on error.
*/
static int	wait_for_child(pid_t pid, int *status)
{
	pid_t	wpid;

	wpid = waitpid(pid, status, 0);
	while (wpid == -1 && errno == EINTR) 
	{
		wpid = waitpid(pid, status, 0);
	}
	return (wpid);
}

/*
 * @brief Extracts the exit status from the wait status.
 *
 * This function interprets the status returned by waitpid to determine
 * the actual exit code of the child process. It handles normal exits
 * and terminations by signals.
 *
 * @param status The status value returned by waitpid.
 * @return The exit code of the child process.
*/
static int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return 0;
}

/**
 * @brief Waits for a child process to finish and handles its exit status.
 *
 * This function waits for the specified child process to terminate and
 * prints a message if it was terminated by a signal. It also returns the
 * exit status of the child process.
 *
 * @param pid The process ID of the child to wait for.
 * @return The exit status of the child process.
 */
int	wait_for_child_and_handle_status(pid_t pid)
{
	int		status;
	pid_t	wpid;

	wpid = wait_for_child(pid, &status);
	if (wpid != -1 && WIFSIGNALED(status))
		print_signal_message(status);
	else if (wpid == -1)
	{
		ft_error("waitpid", strerror(errno));
		return (-1);
	}
	return (get_exit_status(status));
}
