/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 11:09:50 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if all quotes in the input line are properly closed.
 *
 * This function iterates through the input line, tracking the opening
 * and closing of single and double quotes. If it finds any unclosed
 * quotes by the end of the line, it prints a syntax error message.
 *
 * @param line The input line to check.
 * @return 1 if all quotes are properly closed, 0 otherwise.
 */
int	quotes_are_closed(const char *line, t_process_data *pd)
{
	char	quote_char;

	quote_char = 0;
	while (*line != '\0')
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (quote_char && *line == quote_char)
			quote_char = 0;
		line++;
	}
	if (quote_char != 0)
	{
		ft_error("", "syntax error (unclosed quote)");
		if (pd)
			pd->last_exit_status = 2;
		return (0);
	}
	return (1);
}

/**
 * @brief Counts the number of heredoc operators (<<) in a line.
 *
 * This function iterates through the input line, counting occurrences
 * of the heredoc operator (<<) that are not within quotes. It properly
 * handles both single and double quotes to avoid counting operators
 * inside quoted sections.
 *
 * @param line The input line to analyze.
 * @return The number of heredoc operators found in the line.
 */
int	count_heredocs(const char *line)
{
	int		count;
	char	quote_char;

	count = 0;
	quote_char = 0;
	while (line && *line != '\0')
	{
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		else if (*line == quote_char)
			quote_char = 0;
		if (quote_char == 0 && *line == '<' && *(line + 1) == '<')
		{
			count++;
			line++;
		}
		line++;
	}
	return (count);
}

/**
 * @brief Determines the length of a redirection operator.
 *
 * This function checks for single and double-character redirection
 * operators to determine how many characters to skip.
 * @param line The input string.
 * @param i The current index in the string.
 * @return The length of the redirection operator.
 */
static int	get_redir_operator_len(const char *line, int i)
{
	if (line[i] == '<')
	{
		if (line[i + 1] == '<' && line[i + 2] == '<')
			return (3);
		else if (line[i + 1] == '>')
			return (2);
		else if (line[i + 1] == '<')
			return (2);
	}
	else if (line[i] == '>')
	{
		if (line[i + 1] == '>')
			return (2);
	}
	return (1);
}

/**
 * @brief Toggles the quote status based on the current character.
 *
 * This is a helper function that checks if the current character is a quote
 * and updates the quote state accordingly.
 * @param c The character to check.
 * @param quote A pointer to the current quote state.
 */
static void	update_quote_status(char c, char *quote)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

/**
 * @brief Performs a preliminary syntax check on redirections.
 *
 * This function iterates through the command line and checks for
 * syntax errors related to redirections, such as a missing filename.
 * It is run before the main parsing to fail fast on invalid syntax.
 *
 * @param line The command line string to check.
 * @param pd A pointer to the process data struct.
 * @return 1 if the syntax is valid, 0 otherwise.
 */
int	precheck_redir_syntax(const char *line, t_process_data *pd)
{
	int		i;
	char	q;
	int		operator_len;

	if (line == NULL)
		return (1);
	i = 0;
	q = 0;
	while (line[i] != '\0')
	{
		update_quote_status(line[i], &q);
		if (q == 0 && (line[i] == '<' || line[i] == '>'))
		{
			operator_len = get_redir_operator_len(line, i);
			if (!is_valid_token_after_redir(line, i, operator_len, pd))
				return (0);
			i += operator_len;
			continue ;
		}
		i++;
	}
	return (1);
}
