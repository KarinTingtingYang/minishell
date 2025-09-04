/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   precheck_line_helper.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 10:02:01 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 10:09:39 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles syntax error for unexpected newline token.
 *
 * This function prints an error message for an unexpected newline token
 * and updates the last exit status in the process data structure if provided.
 *
 * @param process_data Pointer to the process data structure (can be NULL).
 * @return 0 always to indicate an error.
 */
static int	handle_newline_error(t_process_data *process_data)
{
	ft_error(NULL, "syntax error near unexpected token `newline'");
	if (process_data)
		process_data->last_exit_status = 2;
	return (0);
}

/**
 * @brief Handles syntax error for unexpected pipe token.
 *
 * This function prints an error message for an unexpected pipe token
 * and updates the last exit status in the process data structure if provided.
 *
 * @param process_data Pointer to the process data structure (can be NULL).
 * @return 0 always to indicate an error.
 */
static int	handle_pipe_error(t_process_data *process_data)
{
	ft_error(NULL, "syntax error near unexpected token `|'");
	if (process_data)
		process_data->last_exit_status = 2;
	return (0);
}

/**
 * @brief Handles syntax error for unexpected input redirection token.
 *
 * This function prints an error message for an unexpected input redirection
 * token ('<' or '<<<') and updates the last exit status in the process data
 * structure if provided.
 *
 * @param op_len The length of the redirection operator (1 for '<', 3 for '<<<').
 * @param process_data Pointer to the process data structure (can be NULL).
 * @return 0 always to indicate an error.
 */
static int	handle_input_redir_error(int op_len, t_process_data *process_data)
{
	if (op_len == 3)
		ft_error(NULL, "syntax error near unexpected token `newline'");
	else
		ft_error(NULL, "syntax error near unexpected token `<'");
	if (process_data)
		process_data->last_exit_status = 2;
	return (0);
}

/**
 * @brief Handles syntax error for unexpected output redirection token.
 *
 * This function prints an error message for an unexpected output redirection
 * token ('>') and updates the last exit status in the process data structure
 * if provided.
 *
 * @param process_data Pointer to the process data structure (can be NULL).
 * @return 0 always to indicate an error.
 */
static int	handle_output_redir_error(t_process_data *process_data)
{
	ft_error(NULL, "syntax error near unexpected token `>'");
	if (process_data)
		process_data->last_exit_status = 2;
	return (0);
}

/**
 * @brief Checks for syntax errors related to redirection operators.
 *
 * This function checks the character following a redirection operator
 * to determine if it results in a syntax error. It handles various cases
 * such as unexpected newline, pipe, or another redirection operator.
 *
 * @param line The command line string.
 * @param op_len The length of the redirection operator (1, 2, or 3).
 * @param j The index of the character following the redirection operator.
 * @param process_data Pointer to the process data structure for error handling.
 * @return 1 if no syntax error is found, 0 if an error is detected.
 */
int	check_redir_error(const char *line, int op_len, int j,
	t_process_data *process_data)
{
	if (line[j] == '\0')
		return (handle_newline_error(process_data));
	if (line[j] == '|')
		return (handle_pipe_error(process_data));
	if (line[j] == '<')
		return (handle_input_redir_error(op_len, process_data));
	if (line[j] == '>')
		return (handle_output_redir_error(process_data));
	return (1);
}
