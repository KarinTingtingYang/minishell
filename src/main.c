/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:09:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/05 09:15:12 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// Global flag definition
volatile sig_atomic_t g_child_running = 0;
// int g_last_exit_status = 0;  // Global variable to store the last exit status

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
	char		*input;
	t_env_var	*env_list;
	t_process_data process_data;


	(void)argc;
	(void)argv;

	env_list = init_env(envp);
	if (!env_list)
		return (1);
	process_data.env_list = env_list;
	process_data.last_exit_status = 0;
	setup_signal_handlers();

	while (1)
	{
		g_child_running = 0;
		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		// if (execute_command(input, env_list) == -1)
		if (execute_command(input, env_list, &process_data) == -1)
			ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
		free(input);
	}

	free_env(env_list);
	return (0);
}
