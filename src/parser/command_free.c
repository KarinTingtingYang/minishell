/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:22:23 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:39:28 by makhudon         ###   ########.fr       */
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
