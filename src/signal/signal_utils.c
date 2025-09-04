/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:51:29 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/04 10:02:51 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl+C) in the parent process.
 * 
 * This handler sets a global flag to indicate that a SIGINT was received.
 * It also makes readline behave like bash by moving to a new line, clearing
 * the current input, and redisplaying the prompt.
*/
void	handle_parent_sigint(int signum)
{
	(void)signum;
	g_signal_received = SIGINT;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Prints a message based on how a child process was
 *        terminated by a signal.
 * 
 * If the child was terminated by SIGQUIT, it prints "Quit" and indicates if
 * a core dump occurred. If terminated by SIGINT, it simply prints a newline.
 * 
 * @param status The status code returned by waitpid().
 */
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

/**
 * @brief The event hook for readline. Readline calls this periodically
 * while waiting for input.
 * @return 0 on success.
 */
int	signal_event_hook(void)
{
	if (g_signal_received)
	{
		rl_done = 1;
	}
	return (0);
}
