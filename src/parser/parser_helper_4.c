/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helper_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 11:04:03 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/03 11:15:36 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Prints a syntax error message for invalid redirection.
 *
 * This function formats and prints a syntax error message based on
 * the unexpected token, which can be a token value or "newline".
 *
 * @param unexpected The value of the unexpected token.
 */
static void	print_redirect_error(const char *unexpected)
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
}

/**
 * @brief Validates the syntax of redirection tokens.
 *
 * This function checks for common syntax errors, such as a redirection
 * operator being followed by another redirection operator or the end
 * of the input line.
 *
 * @param tokens The array of tokens to validate.
 * @return 1 if the syntax is valid, 0 otherwise.
 */
static int	is_valid_redirect(t_token **tokens, int i)
{
	const char	*unexpected;

	if (tokens[i + 1] == NULL || is_redirect(tokens[i + 1]->value))
	{
		if (tokens[i + 1] != NULL)
			unexpected = tokens[i + 1]->value;
		else
			unexpected = "newline";
		print_redirect_error(unexpected);
		return (0);
	}
	return (1);
}

/**
 * @brief Checks if a string is a redirection operator.
 *
 * This function checks if the given string matches any of the
 * standard redirection operators: ">", "<", ">>", or "<<".
 *
 * @param s The string to check.
 * @return 1 if the string is a redirection operator, 0 otherwise.
 */
int	is_redirect(const char *s)
{
	if (ft_strncmp(s, ">>", 3) == 0)
		return (1);
	if (ft_strncmp(s, "<<", 3) == 0)
		return (1);
	if (ft_strncmp(s, ">", 2) == 0)
		return (1);
	if (ft_strncmp(s, "<", 2) == 0)
		return (1);
	return (0);
}

/**
 * @brief Validates the syntax of redirection tokens in an array.
 * 
 * This function checks that each redirection operator is followed
 * by a valid token (not another redirection or end of input).
 * @param tokens An array of tokens to validate.
 * @return 1 if all redirection syntax is valid, 0 otherwise.
 */
int	validate_redirect_syntax(t_token **tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens[i] != NULL)
	{
		if (is_redirect(tokens[i]->value))
		{
			if (!is_valid_redirect(tokens, i))
				return (0);
		}
		i++;
	}
	return (1);
}
