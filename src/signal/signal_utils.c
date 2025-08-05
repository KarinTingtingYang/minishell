/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 11:51:29 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/05 08:48:35 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

/**
 * @brief Handles SIGINT signal in the parent process.
 *
 * This function is called when the parent process receives a SIGINT signal.
 * It sets a global flag to indicate that a signal was received, allowing
 * the main loop to handle it appropriately.
 *
 * @param signum The signal number (SIGINT).
 */
void handle_parent_sigint(int signum)
{ //
    // (void)signum; // Cast to void to suppress unused parameter warning

    // // If no child is running, print a new prompt on SIGINT
    // if (g_child_running == 0) { //
    //     ft_putstr_fd("\n", STDOUT_FILENO); // Move to a new line
    //     rl_on_new_line(); // Tell readline that we're on a new line
    //     rl_replace_line("", 0); // Clear the current input buffer
    //     rl_redisplay(); // Redraw the prompt
    // }
    // If a child is running, do nothing.
    // The child will receive SIGINT and terminate, and the parent's waitpid
    // will be interrupted, returning control to the main loop.
	g_signal_received = signum; // Set the global signal received flag
}

void	print_signal_message(int status)
{
    if (WTERMSIG(status) == SIGQUIT)
    {
        ft_putstr_fd("Quit", STDOUT_FILENO);
        if (WCOREDUMP(status))
            ft_putstr_fd(" (core dumped)", STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    }
    else if (WTERMSIG(status) == SIGINT)
    {
        ft_putstr_fd("\n", STDOUT_FILENO);
    }
}