/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_types.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 19:24:59 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/05 14:04:38 by makhudon         ###   ########.fr       */
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

int execute_single_command(char **args, t_env_var *env_list,
                           t_process_data *process_data)
{
    t_execute_data data;
    int            exit_status;

    ft_bzero(&data, sizeof(t_execute_data));
    data.env_list = env_list;
    process_data->in_pipeline = 0;
    data.clean_args = handle_redirection(args, process_data, &data);
    if (!data.clean_args)
        return (handle_redirection_error(&data, process_data, args));
    if (!data.clean_args[0])
        return (handle_redirection_only(&data, process_data));
    if (ft_strncmp(data.clean_args[0], "exit", 5) == 0)
    {
        int       saved_stdin = -1, saved_stdout = -1, did_save = 0;
        long long code;
        if (setup_builtin_io(&data, &saved_stdin, &saved_stdout, &did_save) == 0)
        {
            ft_putstr_fd("exit\n", STDOUT_FILENO);
            if (data.clean_args[1] == NULL)
                code = (unsigned char)process_data->last_exit_status;
            else if (!ft_str_to_llong(data.clean_args[1], &code))
            {
                ft_error_with_arg("exit", data.clean_args[1],
                                  "numeric argument required");
                code = 2;
            }
            else if (data.clean_args[2] != NULL)
            {
                restore_builtin_io(did_save, saved_stdin, saved_stdout);
                free_execute_data(&data);
                process_data->last_exit_status = 1;
                return 1;
            }
            restore_builtin_io(did_save, saved_stdin, saved_stdout);
            free_execute_data(&data);
            if (args) free_split(args);
            rl_clear_history();
            free_env(process_data->env_list);
            exit((unsigned char)code);
        }
        free_execute_data(&data);
        process_data->last_exit_status = 1;
        return 1;
    }
    if (is_builtin(data.clean_args[0]))
        exit_status = execute_builtin_command(&data, process_data);
    else
        exit_status = execute_external_command(&data, process_data, env_list);
    process_data->last_exit_status = exit_status;
    free_execute_data(&data);
    return exit_status;
}
