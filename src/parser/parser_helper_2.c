/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:04:14 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/03 11:14:18 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Reports a syntax error for an unexpected token.
 *
 * This function handles all error reporting for redirection syntax,
 * printing the correct message and updating the exit status.
 * @param unexpected The unexpected token or "newline".
 * @param pd A pointer to the process data struct to update exit status.
 */
static void	report_redir_error(const char *unexpected, t_process_data *pd)
{
	char	*tmp;
	char	*msg;

	tmp = ft_strjoin("syntax error near unexpected token `", unexpected);
	if (tmp == NULL)
		return ;
	msg = ft_strjoin(tmp, "'");
	free(tmp);
	if (msg == NULL)
		return ;
	ft_error(NULL, msg);
	free(msg);
	if (pd)
		pd->last_exit_status = 2;
}

/**
 * @brief Skips spaces after a redirection and checks for a newline.
 *
 * This is a new helper function that advances the pointer past whitespace
 * and returns `0` if a null terminator (newline) is found, indicating
 * a syntax error.
 *
 * @param line The input string.
 * @param i The index after the redirection operator.
 * @param op_len The length of the redirection operator.
 * @param pd A pointer to the process data struct.
 * @return The new index `j` or 0 if a syntax error occurred.
 */
static int	skip_spaces_and_check_for_null(const char *line, int i, int op_len,
												t_process_data *pd)
{
	int	j;

	j = i + op_len;
	while (line[j] == ' ' || line[j] == '\t')
		j++;
	if (line[j] == '\0')
	{
		report_redir_error("newline", pd);
		return (0);
	}
	return (j);
}

/**
 * @brief Checks if the token after a redirection is valid.
 *
 * This function checks for common syntax errors after a redirection operator,
 * such as unexpected pipes or other redirection symbols. It calls a helper
 * function to handle whitespace and null checks.
 *
 * @param line The input string.
 * @param i The index after the redirection operator.
 * @param op_len The length of the redirection operator.
 * @param pd A pointer to the process data struct.
 * @return 1 if valid, 0 if a syntax error.
 */
int	is_valid_token_after_redir(const char *line, int i, int op_len,
											t_process_data *pd)
{
	int	j;

	j = skip_spaces_and_check_for_null(line, i, op_len, pd);
	if (j == 0)
		return (0);
	if (line[j] == '|')
	{
		report_redir_error("|", pd);
		return (0);
	}
	if (line[j] == '<')
	{
		if (op_len == 3)
		{
			report_redir_error("newline", pd);
			return (0);
		}
		report_redir_error("<", pd);
		return (0);
	}
	if (line[j] == '>')
	{
		return (report_redir_error(">", pd), 0);
	}
	return (1);
}
