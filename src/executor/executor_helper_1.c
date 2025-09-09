/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:27:44 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/08 10:51:21 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is NULL, empty, or contains only whitespace.
 * 
 * This function iterates through the string and verifies if all characters
 * are spaces or tabs. If the string is NULL or empty, it also returns true.
 * 
 * @param str The string to check.
 * @return 1 if the string is NULL, empty, or only whitespace; 0 otherwise.
 */
int	is_empty_or_whitespace(const char *str)
{
	if (!str)
		return (1);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	return (*str == '\0');
}

/**
 * @brief Validates the segments of a command pipeline after splitting by '|'.
 * Checks for empty segments which are syntax errors.
 * @param parts The array of command strings.
 * @param count The number of command strings in the array.
 * @return 1 on successful validation, 0 on syntax error.
 */
int	validate_pipeline_parts(char **parts, int count)
{
	int	i;

	if (count <= 1)
		return (1);
	i = 0;
	while (i < count)
	{
		if (is_empty_or_whitespace(parts[i]))
		{
			ft_error("", "syntax error near unexpected token `|'");
			return (0);
		}
		i++;
	}
	return (1);
}

/**
 * @brief Duplicates an array of strings (e.g., command arguments).
 * 
 * This function allocates a new array of strings and copies each string
 * from the original array into the new one. It ensures that the new array
 * is NULL-terminated. If memory allocation fails, it cleans up and returns NULL.
 * @param args The original array of strings to duplicate.
 * @return A newly allocated array of strings, or NULL on failure.
 */
char	**ft_split_dup(char **args)
{
	int		count;
	char	**dup;
	int		i;

	if (args == NULL)
		return (NULL);
	count = count_command_parts(args);
	dup = malloc((count + 1) * sizeof(char *));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < count)
	{
		dup[i] = ft_strdup(args[i]);
		if (dup[i] == NULL)
		{
			while (--i >= 0)
				free(dup[i]);
			free(dup);
			return (NULL);
		}
		i++;
	}
	dup[count] = NULL;
	return (dup);
}

/**
 * @brief Counts the number of command parts in an array of strings.
 * 
 * This function iterates through the array of strings until it finds a NULL
 * terminator, counting the number of valid command parts.
 * @param parts The array of command strings.
 * @return The count of command parts in the array.
 */
int	count_command_parts(char **parts)
{
	int	count;

	count = 0;
	while (parts[count] != NULL)
		count++;
	return (count);
}

/**
 * @brief Checks if an unquoted pipe character '|' is present in the input line.
 *
 * This function iterates through the input line, tracking whether it is
 * currently inside single or double quotes. It returns 1 if an unquoted
 * pipe character is found, and 0 otherwise.
 *
 * @param line The input command line string to check.
 * @return 1 if an unquoted pipe is present, 0 otherwise.
 */
int	is_unquoted_pipe_present(const char *line)
{
	char	quote_char;

	quote_char = 0;
	while (*line)
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (*line == quote_char)
			quote_char = 0;
		else if (quote_char == 0 && *line == '|')
			return (1);
		line++;
	}
	return (0);
}
