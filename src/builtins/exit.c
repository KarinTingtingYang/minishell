/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 13:12:47 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/05 13:38:08 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Parses a string of digits and checks for overflow.
 *
 * This function takes a pointer to the start of a digit sequence, a sign,
 * and a pointer to a long long variable. It converts the digit sequence
 * to a long long, while also checking for overflow and underflow.
 *
 * @param str The string of digits to parse.
 * @param sign The sign of the number (+1 or -1).
 * @param out_val A pointer to store the resulting long long value.
 * @return 1 on success, 0 on failure (e.g., non-digit characters, overflow).
 */
int	ft_parse_digits_to_llong(const char *str,
										int sign, long long *out_val)
{
	long long	result;

	result = 0;
	while (*str != '\0')
	{
		if (!ft_isdigit(*str))
			return (0);
		if (sign == 1)
		{
			if (result > LLONG_MAX / 10
				|| (result == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10))
				return (0);
		}
		else
		{
			if (result > -(LLONG_MIN / 10)
				|| (result == -(LLONG_MIN / 10)
					&& (*str - '0') > -(LLONG_MIN % 10)))
				return (0);
		}
		result = result * 10 + (*str - '0');
		str++;
	}
	*out_val = result * sign;
	return (1);
}

/**
 * @brief Converts a string to a long long, handling whitespace and sign.
 *
 * This is the main function that handles the full conversion logic. It
 * correctly skips leading whitespace and determines the sign before
 * calling ft_parse_digits_to_llong to perform the final conversion.
 *
 * @param str The string to convert.
 * @param out_val A pointer to store the resulting long long value.
 * @return 1 on success, 0 on failure.
 */
int	ft_str_to_llong(const char *str, long long *out_val)
{
	int	sign;

	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str || !ft_isdigit(*str))
		return (0);
	return (ft_parse_digits_to_llong(str, sign, out_val));
}

/**
 * @brief Implements the 'exit' built-in command.
 *
 * This function handles the 'exit' command, which can take an optional
 * numeric argument to specify the exit code. It performs error checking
 * for invalid arguments and too many arguments.
 *
 * @param args The command arguments. args[0] is "exit", args[1] is the
 * optional exit code.
 * @param last_exit_status The last exit status of the shell, used if no
 * argument is provided.
 * @return This function does not return; it calls exit() to terminate the
 * process. If there are too many arguments, it returns 1 without exiting.
 */
int	run_exit(char **args, int last_exit_status)
{
	long long	exit_code;
	int			argc;

	argc = 0;
	while (args && args[argc] != NULL)
		argc++;
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (argc == 1)
		exit((unsigned char)last_exit_status);
	if (ft_str_to_llong(args[1], &exit_code) == 0)
	{
		ft_error_with_arg("exit", args[1], "numeric argument required");
		exit(2);
	}
	if (argc > 2)
	{
		ft_error("exit", "too many arguments");
		return (1);
	}
	exit((unsigned char)exit_code);
	return (0);
}
