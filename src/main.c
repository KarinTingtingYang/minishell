/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/04 13:09:59 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/12 14:08:17 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

volatile sig_atomic_t g_signal_received = 0; // new global variable for signal handling

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
			if (execute_command(input, env_list, &process_data) == -1)
				ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
		}
		free(input);
	}
	free_env(env_list);
	return (0);
}
