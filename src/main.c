/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/04 13:09:59 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/14 13:41:29 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

volatile sig_atomic_t g_signal_received = 0; // new global variable for signal handling

/**
 * @brief This is the new function to handle piped input.
 * It reads commands line-by-line using get_next_line.
 */
static void	run_non_interactive_shell(t_process_data *process_data)
{
	char	*line;

	// Loop as long as get_next_line successfully reads a line from stdin (fd 0).
	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		// get_next_line might include the newline, so we should trim it.
		// A simple way is to find '\n' and replace it with '\0'.
		char *newline_char = ft_strchr(line, '\n');
		if (newline_char)
			*newline_char = '\0';
		
		//printf("minishell (non-interactive) read line: '%s'\n", line);
		
		if (*line) // If the line is not empty
		{
			execute_command(line, process_data->env_list, process_data);
		}
		free(line); // Free the line returned by get_next_line
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_env_var		*env_list;
	t_process_data	process_data;

	(void)argc;
	(void)argv;

	//printf("minishell pid: %d starting initialization\n", getpid());

	env_list = init_env(envp);
	if (!env_list)
	{
        //printf("minishell pid: %d failed env initialization\n", getpid());
        return (1);
    }
	//printf("minishell pid: %d env initialized successfully\n", getpid());
	process_data.env_list = env_list;
	process_data.last_exit_status = 0;
	setup_signal_handlers();
	if (isatty(STDIN_FILENO))
	{
		//printf("minishell pid: %d is interactive, entering main loop\n", getpid());
	while (1)
	{
		//printf("minishell pid: %d about to call readline\n", getpid());
		g_signal_received = 0;
		rl_event_hook = signal_event_hook;
		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
		rl_event_hook = NULL;
		//printf("minishell pid: %d readline returned: %s\n", getpid(), input ? input : "NULL");

		if (input == NULL)
		{
			//printf("minishell pid: %d received EOF, exiting\n", getpid());
			printf("exit\n");
			break;
		}
		if (g_signal_received == SIGINT)
		{
			//printf("minishell pid: %d received SIGINT\n", getpid());
			process_data.last_exit_status = 130;
			rl_on_new_line();
			rl_replace_line("", 0);
			free(input);
			continue;
		}
		
		// --- DEBUGGING: skip leading spaces
        const char *line_ptr = input;
        skip_spaces(&line_ptr);
        if (*line_ptr == '\0')
        {
            free(input);
            continue;
        }
        // -----------------------------------------------------
		if (*input && g_signal_received != SIGINT)
		{
			add_history(input);
			// if (execute_command(input, env_list, &process_data) == -1)
			// {
			// 	// ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO); // DEBUG: Print error if execute_command fails
			// 	ft_error("execute_command", "failed to execute command");
			// }
			int execute_command_result = execute_command(input, env_list, &process_data);
			//printf("Back in main loop of shell %d\n", getpid());
			if (execute_command_result == -1)
			{
				// ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO); // DEBUG: Print error if execute_command fails
				ft_error("execute_command", "failed to execute command");
			}
			
		}
		free(input);
	}
	}
	else
	{
		//printf("minishell pid: %d running in non-interactive mode\n", getpid());
		run_non_interactive_shell(&process_data);
	}
	//printf("minishell pid: %d exiting main loop\n", getpid());
	free_env(env_list);
	return (process_data.last_exit_status);
}
