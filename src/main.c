/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:09:59 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 09:36:50 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/* -------------------------------------------------------------------------- */
/*                               Local utilities                               */
/* -------------------------------------------------------------------------- */

static void	shutdown_shell(t_process_data *process_data)
{
	/* Best-effort cleanup; safe to call multiple times */
	get_next_line_cleanup();
	rl_clear_history();
	if (process_data && process_data->env_list)
	{
		free_env(process_data->env_list);
		process_data->env_list = NULL;
	}
}

/* -------------------------------------------------------------------------- */
/*                          Non-interactive execution                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Runs the non-interactive shell mode, reading commands from stdin.
 *        Ensures each line buffer is freed and no allocations leak.
 */
static void	run_non_interactive_shell(t_process_data *process_data)
{
	char	*line;
	char	*nl;

	while ((line = get_next_line(STDIN_FILENO)) != NULL)
	{
		nl = ft_strchr(line, '\n');
		if (nl)
			*nl = '\0';
		if (*line)
			(void)execute_command(line, process_data->env_list, process_data);
		free(line);
	}
}

/* -------------------------------------------------------------------------- */
/*                            Interactive helpers                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Handles signal interrupts during input reading.
 * @return 1 if a SIGINT was handled (and input freed), 0 otherwise.
 */
static int	handle_signal_interrupt(t_process_data *process_data, char *input)
{
	if (g_signal_received == SIGINT)
	{
		process_data->last_exit_status = 130;
		rl_on_new_line();
		rl_replace_line("", 0);
		free(input);
		return (1);
	}
	return (0);
}

/**
 * @brief Processes a single line of shell input.
 *        Adds to history only after quick syntax precheck.
 */
static int	process_shell_input(char *input, t_process_data *process_data)
{
	const char	*line_ptr;

	line_ptr = input;
	skip_spaces(&line_ptr);
	if (*line_ptr == '\0')
		return (0);
	if (!precheck_redir_syntax(input, process_data))
		return (0);
	add_history(input);
	if (execute_command(input, process_data->env_list, process_data) == -1)
	{
		ft_error("execute_command", "failed to execute command");
		return (0);
	}
	return (1);
}

/* -------------------------------------------------------------------------- */
/*                              Interactive loop                               */
/* -------------------------------------------------------------------------- */

static int	run_interactive_shell(t_process_data *process_data)
{
	char	*input;

	while (1)
	{
		g_signal_received = 0;
		rl_event_hook = signal_event_hook;
		input = readline("\001\033[1;32m\002minishell>\001\033[0m\002 ");
		rl_event_hook = NULL;
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (handle_signal_interrupt(process_data, input))
			continue ;
		if (*input && g_signal_received != SIGINT)
			(void)process_shell_input(input, process_data);
		free(input);
	}
	return (process_data->last_exit_status);
}

/* -------------------------------------------------------------------------- */
/*                                   main                                      */
/* -------------------------------------------------------------------------- */

int	main(int argc, char **argv, char **envp)
{
	t_process_data	process_data;

	(void)argc;
	(void)argv;
	process_data.env_list = init_env(envp);
	bootstrap_env_if_empty(&process_data.env_list);
	process_data.last_exit_status = 0;

	/* Ensure GNL internal buffers are reclaimed on any exit path */
	atexit(get_next_line_cleanup);
	setup_signal_handlers();

	if (isatty(STDIN_FILENO))
		process_data.last_exit_status = run_interactive_shell(&process_data);
	else
		run_non_interactive_shell(&process_data);

	/* Unified shutdown to avoid leaks / still-reachable blocks */
	shutdown_shell(&process_data);
	return (process_data.last_exit_status);
}
