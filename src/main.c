/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:09:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 09:45:00 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

// Global flag definition
volatile sig_atomic_t g_child_running = 0;

int	main(int argc, char **argv, char **envp)
{
	char	*input;

	(void)argc;
	(void)argv;
	setup_signal_handlers(); // set up the handlers for the parent process
	while (1)
	{
		g_child_running = 0; // No child running while waiting for input
		input = readline("\033[1;32mminishell>\033[0m ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strncmp(input, "exit", ft_strlen("exit") + 1) == 0)
		{
			printf("exit\n");
			free(input);
			break ;
		}
		if (execute_command(input, envp) == -1)
			ft_putstr_fd("Error: failed to execute command\n", STDERR_FILENO);
		free(input);
	}
	return (0);
}
