/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:15:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 10:16:00 by makhudon         ###   ########.fr       */
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
 * @brief Skips leading spaces and tabs in a string.
 * 
 * This function advances the pointer to the first non-space,
 * non-tab character in the string.
 * @param s A pointer to the string pointer to modify.
 */
void	skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
		(*s)++;
}
