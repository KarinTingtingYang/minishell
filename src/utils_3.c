/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 12:47:35 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 13:03:54 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 * 
 * This function iterates through the array, freeing each individual
 * string, and then frees the array itself.
 * @param array The NULL-terminated array of strings to free.
 */
void	free_split(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/**
 * @brief Safely frees a pointer and sets it to NULL.
 * 
 * This function checks if the pointer is non-NULL before freeing it,
 * and then sets the pointer to NULL to avoid dangling references.
 * @param p A pointer to the pointer to be freed.
 */
void	free_ptr(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
	}
}

/**
 * @brief Frees the file and path-related members of the execute data structure.
 * * This function cleans up the file paths and command path, including unlinking
 * and freeing the heredoc file if it exists.
 *
 * @param data A pointer to the execute data structure.
 */
static void	free_execute_files_and_paths(t_execute_data *data)
{
	if (data->heredoc_file != NULL)
	{
		unlink(data->heredoc_file);
		free(data->heredoc_file);
		data->heredoc_file = NULL;
	}
	free_ptr((void **)&data->input_file);
	free_ptr((void **)&data->output_file);
	if (data->path_dirs)
	{
		free_split(data->path_dirs);
		data->path_dirs = NULL;
	}
	free_ptr((void **)&data->cmd_path);
}

/**
 * @brief Frees the argument arrays from the execute data structure.
 * * This function is responsible for cleaning up the `original_args` and
 * `clean_args` arrays.
 *
 * @param data A pointer to the execute data structure.
 */
static void	free_execute_arguments(t_execute_data *data)
{
	if (data->original_args)
	{
		free_split(data->original_args);
		data->original_args = NULL;
	}
	if (data->clean_args)
	{
		free_split(data->clean_args);
		data->clean_args = NULL;
	}
}

/**
 * @brief Frees all dynamically allocated members of the execute data structure.
 * * This function acts as a central cleanup utility, delegating to helper
 * functions to free specific groups of members, such as file paths and
 * argument arrays.
 *
 * @param data A pointer to the execute data structure to be freed.
 */
void	free_execute_data(t_execute_data *data)
{
	if (data == NULL)
		return ;   
	free_execute_files_and_paths(data);
	free_execute_arguments(data);
}
