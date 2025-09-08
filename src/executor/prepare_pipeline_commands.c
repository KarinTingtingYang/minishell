/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_pipeline_commands.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 16:42:20 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/08 10:50:24 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Allocates memory for the command array.
 *
 * @param count The number of commands.
 * @param cmds Pointer to the command array.
 * @return 1 on success, 0 on failure.
 */
static int	allocate_command_array(int count, t_command ***cmds)
{
	*cmds = malloc(sizeof(t_command *) * ((size_t)count + 1));
	if (*cmds == NULL)
		return (0);
	ft_bzero(*cmds, sizeof(t_command *) * ((size_t)count + 1));
	return (1);
}

/**
 * @brief Validates the command line and allocates necessary structures.
 *
 * This function performs a precheck on the command line, splits it by pipes,
 * counts the number of command parts, and validates the syntax of the pipeline.
 * If any step fails, it cleans up allocated resources and returns 0.
 *
 * @param line The command line to validate.
 * @param count Pointer to store the number of command parts.
 * @param parts Pointer to store the split command parts.
 * @param data Pointer to the process data structure for syntax error tracking.
 * @return 1 on success, 0 on failure.
 */
static int	validate_and_allocate(char *line, int *count, char ***parts,
	t_process_data *data)
{
	if (!precheck_line(line, data))
		return (0);
	*parts = split_line_by_pipe(line);
	if (*parts == NULL || (*parts)[0] == NULL)
		return (0);
	*count = count_command_parts(*parts);
	if (data)
		data->syntax_error = 0;
	if (!validate_pipeline_parts(*parts, *count))
	{
		free_split(*parts);
		*parts = NULL;
		return (0);
	}
	return (1);
}

/**
 * @brief Prepares the pipeline commands.
 *
 * This function validates the command line, splits it by pipes,
 * allocates the command array, and builds the command structures
 * from the split parts. It returns the command array on success
 * or NULL on failure.
 *
 * @param line The command line.
 * @param count Pointer to the number of commands.
 * @param parts Pointer to the array of command parts.
 * @param data Pointer to the process data.
 * @return Pointer to the command array, or NULL on failure.
 */
t_command	**prepare_pipeline_commands(char *line, int *count, char ***parts,
	t_process_data *data)
{
	if (!validate_and_allocate(line, count, parts, data))
		return (NULL);
	if (!allocate_command_array(*count, &data->cmds))
	{
		free_split(*parts);
		return (NULL);
	}
	if (!build_commands_from_parts(*parts, *count, data))
	{
		free_commands_recursive(data->cmds, 0, *count);
		free(data->cmds);
		free_split(*parts);
		return (NULL);
	}
	data->cmds[*count] = NULL;
	return (data->cmds);
}
