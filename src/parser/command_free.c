/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:22:23 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/13 10:13:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees the memory allocated for a command structure.
 *
 * This function frees all dynamically allocated fields in the `t_command`
 * structure, including the command path, arguments, input/output files,
 * and heredoc file. It does not free the structure itself.
 * @param cmd Pointer to the command structure to free.
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
