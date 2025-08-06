/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:09:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/06 09:38:49 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// Global flag definition
// volatile sig_atomic_t g_child_running = 0;
volatile sig_atomic_t g_signal_received = 0; // new global variable for signal handling
//int g_last_exit_status = 0;

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
// int	main(int argc, char **argv, char **envp)
// {
// 	char		*input;
// 	t_env_var	*env_list;
// 	t_process_data process_data;


// 	(void)argc;
// 	(void)argv;

// 	env_list = init_env(envp);
// 	if (!env_list)
// 		return (1);
// 	process_data.env_list = env_list;
// 	process_data.last_exit_status = 0;
// 	setup_signal_handlers();

// 	while (1)
// 	{
// 		//g_child_running = 0;
// 		g_signal_received = 0; // Reset signal received flag for each new prompt
// 		// Set the event hook before calling readline.
// 		rl_event_hook = signal_event_hook;
// 		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
// 		// Unset the hook immediately after.
// 		rl_event_hook = NULL;
// 		// After readline, check if it was interrupted by our handler.
// 		if (g_signal_received == SIGINT)
// 		{
// 			process_data.last_exit_status = 130; // Set exit status for SIGINT
// 			rl_on_new_line();                 // Tell readline we are on a new line.
// 			rl_replace_line("", 0);           // Clear readline's internal buffer.
// 			free(input);                      // Free the empty string from the interrupted readline.
// 			continue;                         // Skip the rest of the loop and show a fresh prompt.
// 		}
// 		if (input == NULL)
// 		{  // Ctrl+D was pressed
// 			printf("exit\n");
// 			break ;
// 		}
// 		// If the command was not interrupted, proceed.
// 		if (*input && g_signal_received != SIGINT)
// 		{
// 			add_history(input);
// 			char **args = parse_line(input);
// 			if (args)
//     		{
// 				// if (execute_command(input, env_list) == -1)
// 				if (execute_command(input, env_list, &process_data) == -1)
// 					ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
// 				free_split(args); // Free after use
// 			}
// 		}
// 		free(input);
// 	}

// 	free_env(env_list);
// 	return (0);
// }

#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env_var		*env_list;
	t_process_data	process_data;

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
		g_signal_received = 0;
		rl_event_hook = signal_event_hook;
		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
		rl_event_hook = NULL;
		if (g_signal_received == SIGINT)
		{
			process_data.last_exit_status = 130;
			rl_on_new_line();
			rl_replace_line("", 0);
			free(input);
			continue;
		}
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		if (*input && g_signal_received != SIGINT)
		{
			add_history(input);
			if (execute_command(input, env_list, &process_data) == -1)
				ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
		}
		free(input);
	}
	free_env(env_list);
	return (0);
}
