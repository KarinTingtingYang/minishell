/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   precheck_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/01 19:51:29 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 10:32:33 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Updates the quote state based on the current character.
 *
 * This function toggles the quote state when encountering single or double
 * quotes, ensuring that redirection operators within quotes are ignored.
 *
 * @param c The current character being processed.
 * @param q Pointer to the current quote state (0 if not in quotes, otherwise
 *          holds the quote character).
 */
static void	update_quote_state(char c, char *q)
{
	if ((c == '\'' || c == '"') && *q == 0)
		*q = c;
	else if (*q && c == *q)
		*q = 0;
}

/**
 * @brief Determines the length of a redirection operator 
 * starting at a given index.
 *
 * This function checks for single and double redirection operators ('<', '<<',
 * '<<<', '>', '>>') and returns their length.
 *
 * @param line The command line string.
 * @param i The index to check for a redirection operator.
 * @return The length of the redirection operator (1, 2, or 3).
 */
static int	get_redir_op_len(const char *line, int i)
{
	if (line[i] == '<')
	{
		if (line[i + 1] == '<' && line[i + 2] == '<')
			return (3);
		if (line[i + 1] == '>' || line[i + 1] == '<')
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
 * @brief Skips spaces and tabs in the command line starting from a given index.
 *
 * This function advances the index past any spaces or tabs to find the next
 * non-whitespace character.
 *
 * @param line The command line string.
 * @param j The starting index to skip spaces from.
 * @return The index of the next non-whitespace character.
 */
static int	skip_spaces_in_precheck(const char *line, int j)
{
	while (line[j] == ' ' || line[j] == '\t')
		j++;
	return (j);
}

/**
 * @brief Performs a precheck on the command line for bad redirections.
 *
 * This function scans the command line for redirection operators 
 * ('<', '>', etc.)
 * that are not enclosed in quotes and checks for common syntax errors, such as
 * missing filenames or invalid tokens following the operators.
 *
 * @param line The command line string to check.
 * @param process_data Pointer to the process data structure for error handling.
 * @return 1 if no syntax errors are found, 0 if an error is detected.
 */
int	precheck_line(char *line, t_process_data *process_data)
{
	int		i;
	char	q;
	int		op_len;
	int		j;

	i = 0;
	q = 0;
	if (!line)
		return (1);
	while (line[i] != '\0')
	{
		update_quote_state(line[i], &q);
		if (q == 0 && (line[i] == '<' || line[i] == '>'))
		{
			op_len = get_redir_op_len(line, i);
			j = skip_spaces_in_precheck(line, i + op_len);
			if (!check_redir_error(line, op_len, j, process_data))
				return (0);
			i = j;
			continue ;
		}
		i++;
	}
	return (1);
}

// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------
// /**
//  * @brief Performs a precheck on the raw line for bad redirections.
//  *
//  * @param line The command line.
//  * @param process_data Pointer to the process data.
//  * @return 1 on success, 0 on failure.
//  */
// int precheck_line(char *line, t_process_data *process_data)
// {
//     if (line)
//     {
//         int i = 0;
//         char q = 0;

//         while (line[i] != '\0')
//         {
//             if ((line[i] == '\'' || line[i] == '"') && q == 0)
//             {
//                 q = line[i];
//                 i++;
//                 continue;
//             }
//             if (q && line[i] == q)
//             {
//                 q = 0;
//                 i++;
//                 continue;
//             }

//             if (q == 0 && (line[i] == '<' || line[i] == '>'))
//             {
//                 int op_len;
//                 int j;

//                 op_len = 1;
//                 if (line[i] == '<')
//                 {
//                     if (line[i + 1] == '<' && line[i + 2] == '<')
//                         op_len = 3;
//                     else if (line[i + 1] == '>')
//                         op_len = 2;
//                     else if (line[i + 1] == '<')
//                         op_len = 2;
//                 }
//                 else
//                 {
//                     if (line[i + 1] == '>')
//                         op_len = 2;
//                 }

//                 j = i + op_len;

//                 while (line[j] == ' ' || line[j] == '\t')
//                     j++;

//                 if (line[j] == '\0')
//                 {
//                     ft_error(NULL, "syntax error near unexpected token `newline'");
//                     if (process_data)
//                         process_data->last_exit_status = 2;
//                     return 0;
//                 }
//                 if (line[j] == '|')
//                 {
//                     ft_error(NULL, "syntax error near unexpected token `|'");
//                     if (process_data)
//                         process_data->last_exit_status = 2;
//                     return 0;
//                 }
//                 if (line[j] == '<')
//                 {
//                     if (op_len == 3)
//                     {
//                         ft_error(NULL, "syntax error near unexpected token `newline'");
//                         if (process_data)
//                             process_data->last_exit_status = 2;
//                         return 0;
//                     }
//                     ft_error(NULL, "syntax error near unexpected token `<'");
//                     if (process_data)
//                         process_data->last_exit_status = 2;
//                     return 0;
//                 }
//                 if (line[j] == '>')
//                 {
//                     ft_error(NULL, "syntax error near unexpected token `>'");
//                     if (process_data)
//                         process_data->last_exit_status = 2;
//                     return 0;
//                 }

//                 i = j;
//                 continue;
//             }

//             i++;
//         }
//     }
//     return 1;
// }

