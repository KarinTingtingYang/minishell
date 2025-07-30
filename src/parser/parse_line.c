/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 08:35:59 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Parses a command line string into an array of arguments.
 * 
 * Splits the input line by spaces into an array of strings.
 * Returns NULL if the input line is NULL or empty.
 * @param line  The input command line string to parse.
 * @return A NULL-terminated array of strings (arguments),
 *         or NULL if input is empty.
 */
char **parse_line(char *line)
{
	if (line == NULL || *line == '\0')
		return (NULL);
	return (ft_split(line, ' '));
}



