/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 11:22:23 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 08:35:50 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees all dynamically allocated memory in a t_command structure.
 * 
 * This function safely frees all parts of a t_command object, including:
 * - the argument array,
 * - the resolved command path,
 * - input and output redirection file strings,
 * - and finally the command structure itself.
 * If the input `cmd` is NULL, the function does nothing.
 * @param cmd Pointer to the t_command structure to free.
 */
void free_command(t_command *cmd)
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
	free(cmd);
}
