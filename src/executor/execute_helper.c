/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 10:33:29 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 10:41:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes an external command by finding its full path and running it.
 *
 * This function retrieves the PATH directories 
 * from the environment variable list,
 * finds the full path of the command, and then executes it using the prepared
 * command execution function.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @param env_list The environment variable list.
 * @return The exit status of the executed external command.
 */
int	execute_external_command(t_execute_data *data, t_process_data *process_data,
								t_env_var *env_list)
{
	char	**path_dirs;
	int		exit_status;

	path_dirs = find_path_dirs(env_list);
	data->cmd_path = find_full_cmd_path(data->clean_args[0], path_dirs);
	if (data->cmd_path == NULL)
	{
		ft_error(data->clean_args[0], "command not found");
		exit_status = 127;
		process_data->last_exit_status = exit_status;
		free_split(path_dirs);
		return (exit_status);
	}
	free_split(path_dirs);
	exit_status = execute_prepared_command(data, process_data);
	return (exit_status);
}

/**
 * @brief Handles cases where only redirection is specified without a command.
 *
 * This function checks if the input file can be opened for reading and
 * sets the last exit status accordingly. It frees the execute data structure
 * before returning.
 *
 * @param data Pointer to the execute data structure containing command info.
 * @param process_data Pointer to the global process data structure.
 * @return 0 if successful, or 1 if there was an error opening the input file.
 */
int	handle_redirection_only(t_execute_data *data, t_process_data *process_data)
{
	int	exit_status;
	int	fd;

	exit_status = 0;
	if (data->input_file)
	{
		fd = open(data->input_file, O_RDONLY);
		if (fd == -1)
		{
			ft_error(data->input_file, strerror(errno));
			exit_status = 1;
		}
		else
		{
			close(fd);
		}
	}
	process_data->last_exit_status = exit_status;
	free_execute_data(data);
	return (exit_status);
}
