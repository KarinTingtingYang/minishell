/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cleanup_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:55:21 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:21:07 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Cleans up command and pipeline-related data from
 *        the process data structure.
 *
 * This function frees the command structures, PID array, and split parts
 * associated with the command execution. It sets the pointers to NULL and
 * resets the command count to avoid dangling references.
 *
 * @param data A pointer to the process data structure.
 */
static void	cleanup_command_pipeline_data(t_process_data *data)
{
	if (data->cmds)
	{
		free_commands_range(data->cmds, data->cmd_count);
		free_ptr((void **)&data->cmds);
		data->cmd_count = 0;
	}
	free_ptr((void **)&data->pids);
	if (data->parts)
	{
		free_split(data->parts);
		data->parts = NULL;
	}
}

/**
 * @brief Cleans up shell environment data from the process data structure.
 *
 * This function frees the `path_dirs` array and the `env_list` linked list
 * associated with the shell environment. It sets the pointers to NULL after
 * freeing to avoid dangling references.
 *
 * @param data A pointer to the process data structure.
 */
static void	cleanup_shell_environment_data(t_process_data *data)
{
	if (data->path_dirs)
	{
		free_split(data->path_dirs);
		data->path_dirs = NULL;
	}
	if (data->env_list)
	{
		free_env(data->env_list);
		data->env_list = NULL;
	}
}

/**
 * @brief Cleans up all resources used in child process execution.
 *
 * This function is called in the child process to free all resources
 * associated with the pipeline execution, including command data,
 * environment data, and any other allocated resources.
 *
 * @param data A pointer to the process data structure.
 */
void	cleanup_child_pipeline_resources(t_process_data *data)
{
	if (!data)
		return ;
	get_next_line_cleanup();
	cleanup_command_pipeline_data(data);
	cleanup_shell_environment_data(data);
}

/**
 * @brief Free both args and envp if present.
 */
void	free_args_env(char **args, char **envp)
{
	if (args)
		free_split(args);
	if (envp)
		free_split(envp);
}

/**
 * @brief Handles the case when cmd_path is NULL.
 *
 * This function frees the provided args and envp arrays, then
 * exits with an appropriate error message and status code.
 *
 * @param args The command arguments array to free.
 * @param envp The environment variables array to free.
 */
void	handle_null_cmd_path(char **args, char **envp)
{
	char		namebuf[PATH_MAX];
	int			has_slash;
	const char	*cmd_name_to_print;

	has_slash = (args && args[0] && ft_strchr(args[0], '/') != NULL);
	if (args && args[0])
		ft_strlcpy(namebuf, args[0], sizeof(namebuf));
	free_args_env(args, envp);
	if (has_slash)
	{
		ft_error_and_exit(namebuf, "No such file or directory", 127);
	}
	else
	{
		if (namebuf[0] != '\0')
		{
			cmd_name_to_print = namebuf;
		}
		else
		{
			cmd_name_to_print = "minishell";
		}
		ft_error_and_exit((char *)cmd_name_to_print,
			"command not found", 127);
	}
}
