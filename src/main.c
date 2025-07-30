/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:09:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 08:37:40 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// Global flag definition
volatile sig_atomic_t g_child_running = 0;

/**
 * @brief  Main loop for the minishell program, handling user input and
 *         command execution.
 * Sets up signal handlers for the shell, then repeatedly prompts the user for
 * input. Reads commands from the standard input, adds non-empty commands to
 * history, processes built-in "exit" command to quit, and executes other commands
 * via execute_command(). Handles graceful exit on EOF or "exit" command.
 * @param argc  Argument count from the command line (ignored).
 * @param argv  Argument vector from the command line (ignored).
 * @param envp  Environment variables array passed to commands.
 * @return Returns 0 on normal shell exit.
 */
int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	setup_signal_handlers(); // set up the handlers for the parent process
	while (1)
	{
		g_child_running = 0; // No child running while waiting for input
		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		// if (ft_strncmp(input, "exit", ft_strlen("exit") + 1) == 0)
		// {
		// 	printf("exit\n");
		// 	free(input);
		// 	break ;
		// } // exiting the shell is handled by the built-in exit command
		if (execute_command(input, envp) == -1)
			ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
		free(input);
	}
	return (0);
}
