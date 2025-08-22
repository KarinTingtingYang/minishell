/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 13:12:47 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/22 09:45:02 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Safely converts a string to a long long, checking for overflow.
 *
 * This function converts a string to a long long integer. It handles whitespace,
 * signs, and crucially, checks for overflow before it can occur.
 *
 * @param str The string to convert.
 * @param out_val A pointer to a long long where the result will be stored.
 * @return Returns 1 on successful conversion, and 0 if the string is not a
 * valid number or if the number would overflow the long long type.
 */
static int	ft_str_to_llong(const char *str, long long *out_val)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	while (*str != '\0')
	{
		if (!ft_isdigit(*str))
			return (0);
		if (sign == 1 && (result > LLONG_MAX / 10 ||
			(result == LLONG_MAX / 10 && (*str - '0') > LLONG_MAX % 10)))
			return (0); // Overflow
		if (sign == -1 && (result > -(LLONG_MIN / 10) ||
			(result == -(LLONG_MIN / 10) && (*str - '0') > -(LLONG_MIN % 10))))
			return (0); // Overflow
		result = result * 10 + (*str - '0');
		str++;
	}
	*out_val = result * sign;
	return (1);
}

/**
 * @brief Exits the shell with the specified exit status.
 * 
 * This function is called when the user types `exit` in the shell.
 * It prints a message indicating the exit status and terminates the program.
 * @param status The exit status to return to the operating system.
 */
int run_exit(char **args, int last_exit_status)
{
	long long exit_code;

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (args[1] == NULL) // Case 1: Just "exit"
		exit((unsigned char)last_exit_status); // EXIT CODE DEBUG: Exit with last exit status if no argument is provided
	else // Case 2: exit with argument(s)
	{
		// try to convert the first argument to a number
		if (ft_str_to_llong(args[1], &exit_code) == 0)
		{
			// ft_putstr_fd("minishell: exit: ", STDERR_FILENO); // DEBUG: Print error if conversion fails
			// ft_putstr_fd(args[1], STDERR_FILENO); // DEBUG: Print the invalid argument
			// ft_putstr_fd(": numeric argument required\n", STDERR_FILENO); // DEBUG: Print the error message
			// exit(2); // Standard failure code for this error // DEBUG: Exit with status 2 if the argument is not a valid number
			ft_error_and_exit("exit", "numeric argument required", 2);
		}
		// If the conversion is successful, check if there are more arguments
		if (args[2] != NULL) // Case 2.1: "exit" with too many arguments
		{
			// ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO); // DEBUG: Print error if too many arguments
			ft_error("exit", "too many arguments");
			return 1; // Return 1 to indicate an error, do not exit
		}
		else // Case 2.2: "exit" with one argument that is a valid number
			exit((unsigned char)exit_code); 
	}
	return 0; // This line is never reached, but added for completeness
}