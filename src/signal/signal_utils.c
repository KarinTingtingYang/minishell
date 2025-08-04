/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:51:29 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/30 11:48:22 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
/**
 * @brief Signal handler for SIGINT (Ctrl+C) in the parent process.
 *
 * This handler is designed to behave differently based on whether a child process
 * is currently executing.
 * - If no child is running (g_child_running == 0), it prints a new prompt.
 * - If a child is running (g_child_running == 1), it does nothing. This allows
 * the SIGINT to terminate the child, and the parent's waitpid will be
 * interrupted, leading it back to the prompt after the child exits.
 *
 * @param signum The signal number (expected to be SIGINT).
 */
void handle_parent_sigint(int signum)
{ //
    (void)signum; // Cast to void to suppress unused parameter warning

    // If no child is running, print a new prompt on SIGINT
    if (g_child_running == 0) { //
        ft_putstr_fd("\n", STDOUT_FILENO); // Move to a new line
        rl_on_new_line(); // Tell readline that we're on a new line
        rl_replace_line("", 0); // Clear the current input buffer
        rl_redisplay(); // Redraw the prompt
    }
    // If a child is running, do nothing.
    // The child will receive SIGINT and terminate, and the parent's waitpid
    // will be interrupted, returning control to the main loop.
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