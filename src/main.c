/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/04 13:09:59 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/08 09:47:41 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

/**
*@brief Runs the non-interactive shell mode, reading commands
        from standard input.
*@param process_data Pointer to the process data structure.
*/
static void	run_non_interactive_shell(t_process_data *process_data)
{
	char	*line;
	char	*newline_char;

	line = get_next_line(STDIN_FILENO);
	while ((line != NULL))
	{
		newline_char = ft_strchr(line, '\n');
		if (newline_char)
			*newline_char = '\0';
		if (*line)
			execute_command(line, process_data->env_list, process_data);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}

/**
*@brief Handles signal interrupts during input reading.
*@param process_data Pointer to the process data structure.
*@param input The input line to process.
*@return 1 if a SIGINT was handled, 0 otherwise.
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
*@brief Processes a single line of shell input.
*@param input The input line to process.
*@param process_data Pointer to the process data structure.
*@return 1 if the command was processed, 0 otherwise.
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

/**
*@brief Runs the interactive shell loop.
*@param process_data Pointer to the process data structure.
*@return Exit status of the shell.
*/
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
			process_shell_input(input, process_data);
		free(input);
	}
	return (process_data->last_exit_status);
}

/**
*@brief Main entry point for the minishell program.
*@param argc Argument count.
*@param argv Argument vector.
*@param envp Environment variables.
*@return Exit status of the shell.
*/
int	main(int argc, char **argv, char **envp)
{
	t_env_var		*env_list;
	t_process_data	process_data;
	//int				exit_status;

	(void)argc;
	(void)argv;
	env_list = init_env(envp);
	if (!env_list)
		return (1);
	process_data.env_list = env_list;
	process_data.last_exit_status = 0;
	setup_signal_handlers();
	if (isatty(STDIN_FILENO))
		//exit_status = run_interactive_shell(&process_data); //DEBUG
		process_data.last_exit_status = run_interactive_shell(&process_data);
	else
		run_non_interactive_shell(&process_data);
	rl_clear_history();//DEBUG
	free_env(env_list);
	return (process_data.last_exit_status);
}
