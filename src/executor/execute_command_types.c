/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_types.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:24:59 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/08 10:39:58 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Determines the exit code based on the 'exit' command's arguments.
 *
 * This function validates the arguments to the `exit` command. It handles
 * cases with no arguments, one numeric argument, and too many arguments,
 * returning the appropriate exit code or an error status.
 *
 * @param data The execution data with the cleaned arguments.
 * @param process_data The process data, used for the last exit status.
 * @return The final exit code, or 1 if there are too many arguments.
 */
static int	get_exit_code(t_execute_data *data, t_process_data *process_data)
{
	long long	code;

	if (data->clean_args[1] == NULL)
	{
		return ((unsigned char)process_data->last_exit_status);
	}
	else if (!ft_str_to_llong(data->clean_args[1], &code))
	{
		ft_error_with_arg("exit", data->clean_args[1],
			"numeric argument required");
		return (2);
	}
	else if (data->clean_args[2] != NULL)
	{
		ft_putstr_fd("exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else
	{
		return ((unsigned char)code);
	}
}

/**
 * @brief Cleans up all memory and exits the shell with the given code.
 *
 * This function consolidates all the final cleanup tasks, ensuring memory
 * is freed correctly before the program terminates.
 *
 * @param data The execution data to free.
 * @param process_data The process data with the environment list to free.
 * @param args The original arguments to free.
 * @param code The exit code for the program.
 */
static void	cleanup_and_exit(t_execute_data *data,
						t_process_data *process_data, char **args, int code)
{
	free_execute_data(data);
	if (args)
		free_split(args);
	rl_clear_history();
	free_env(process_data->env_list);
	exit(code);
}

/**
 * @brief Handles the 'exit' built-in command.
 *
 * This is the main orchestrator for the `exit` command. It sets up I/O
 * redirection, determines the exit code, and then calls the cleanup
 * function to terminate the program.
 *
 * @param data The execution data structure.
 * @param process_data The process data, used for the last exit status.
 * @param args The command arguments.
 * @return 1 on failure (e.g., too many arguments), otherwise exits the program.
 */
int	handle_exit_builtin(t_execute_data *data, t_process_data *process_data,
								char **args)
{
	int	saved_stdin;
	int	saved_stdout;
	int	did_save;
	int	exit_code;

	saved_stdin = -1;
	saved_stdout = -1;
	did_save = 0;
	if (setup_builtin_io(data, &saved_stdin, &saved_stdout, &did_save) != 0)
	{
		return (free_execute_data(data), process_data->last_exit_status = 1, 1);
	}
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	exit_code = get_exit_code(data, process_data);
	if (exit_code == 1)
	{
		restore_builtin_io(did_save, saved_stdin, saved_stdout);
		free_execute_data(data);
		process_data->last_exit_status = 1;
		return (1);
	}
	restore_builtin_io(did_save, saved_stdin, saved_stdout);
	cleanup_and_exit(data, process_data, args, exit_code);
	return (0);
}

/**
 * @brief Executes a command, determining if it's a built-in or external program.
 *
 * This function acts as a dispatcher, calling the correct execution function
 * based on whether the command name is a known built-in.
 *
 * @param data The execution data structure.
 * @param process_data The process data, used to set the last exit status.
 * @param env_list The environment variable list.
 * @return The exit status of the executed command.
 */
static int	execute_builtin_or_external(t_execute_data *data,
							t_process_data *process_data, t_env_var *env_list)
{
	int	exit_status;

	if (is_builtin(data->clean_args[0]))
		exit_status = execute_builtin_command(data, process_data);
	else
		exit_status = execute_external_command(data, process_data, env_list);
	return (exit_status);
}

/**
 * @brief Executes a single command.
 *
 * This is the main orchestrator function. It handles argument
 * cleanup and redirection, then dispatches the command to the
 * appropriate handler (either the built-in
 * exit command or a generic built-in/external executor).
 *
 * @param args The original command arguments.
 * @param env_list The environment variable list.
 * @param process_data The process data structure.
 * @return The exit status of the executed command.
 */
int	execute_single_command(char **args, t_env_var *env_list,
								t_process_data *process_data)
{
	t_execute_data	data;
	int				exit_status;

	ft_bzero(&data, sizeof(t_execute_data));
	data.env_list = env_list;
	process_data->in_pipeline = 0;
	data.clean_args = handle_redirection(args, process_data, &data);
	if (!data.clean_args)
		return (handle_redirection_error(&data, process_data, args));
	if (!data.clean_args[0])
		return (handle_redirection_only(&data, process_data));
	if (ft_strncmp(data.clean_args[0], "exit", 5) == 0)
		exit_status = handle_exit_builtin(&data, process_data, args);
	else
		exit_status = execute_builtin_or_external(&data,
				process_data, env_list);
	process_data->last_exit_status = exit_status;
	free_execute_data(&data);
	return (exit_status);
}
