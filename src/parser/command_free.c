/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:22:23 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 09:57:02 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 *
 * This function iterates through the array, freeing each individual
 * string, and then frees the array itself.
 *
 * @param split The NULL-terminated array of strings to free.
 */
void	free_command(t_command *cmd)
{
	if (cmd == NULL)
		return ;
	free_split(cmd->args);
	if (cmd->cmd_path != NULL)
		free(cmd->cmd_path);
	if (cmd->input_file != NULL)
		free(cmd->input_file);
	if (cmd->output_file != NULL)
		free(cmd->output_file);
	if (cmd->heredoc_file != NULL)
	{
		unlink(cmd->heredoc_file);
		free(cmd->heredoc_file);
	}
	free(cmd);
}

/**
 * @brief Frees allocated resources and returns an error code.
 *
 * This function centralizes the cleanup for redirection parsing failures.
 * It frees allocated memory for input, output, and heredoc files, and
 * unlinks the heredoc file from the filesystem.
 *
 * @param exec_data The structure containing the file paths to free.
 * @return Returns -1 to signal a failure to the calling function.
 */
int	cleanup_and_return_error(t_execute_data exec_data)
{
	free(exec_data.input_file);
	free(exec_data.output_file);
	if (exec_data.heredoc_file)
	{
		unlink(exec_data.heredoc_file);
		free(exec_data.heredoc_file);
	}
	return (-1);
}
