/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:03:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 12:29:39 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Prints a string with special characters escaped for export.
 *
 * This function prints the given string to standard output, escaping
 * special characters like double quotes, backslashes, and dollar signs
 * with a preceding backslash.
 *
 * @param s The string to print with escapes.
 */
static void	put_escaped_export_value(const char *s)
{
	size_t	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '"' || s[i] == '\\' || s[i] == '$')
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(s[i], STDOUT_FILENO);
		i++;
	}
}

/**
 * @brief Sorts an environment variable linked list using bubble sort.
 *
 * This function repeatedly calls perform_single_pass until no swaps
 * are made in a full pass, indicating the list is sorted.
 *
 * @param start The head of the environment variable linked list.
 */
void	bubble_sort_env_list(t_env_var *start)
{
	int	swapped;

	if (start == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = perform_single_pass(start);
	}
}

/**
 * @brief Displays all environment variables in sorted order.
 *
 * This function duplicates the environment variable list, sorts it,
 * and prints each variable in the format required by the `export` command.
 * It frees the duplicated list after printing.
 *
 * @param env_list The head of the environment variable linked list.
 */
static void	display_export(t_env_var *env_list)
{
	t_env_var	*sorted_list;
	t_env_var	*cur;

	sorted_list = duplicate_env_list(env_list);
	bubble_sort_env_list(sorted_list);
	cur = sorted_list;
	while (cur != NULL)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(cur->key, STDOUT_FILENO);
		if (cur->value != NULL)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			put_escaped_export_value(cur->value);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		cur = cur->next;
	}
	free_env(sorted_list);
}

/**
 * @brief Implements the 'export' built-in command.
 * Adds or updates environment variables, or displays all variables.
 *
 * @param env_list The linked list of environment variables.
 * @param args The command arguments. args[0] is "export", args[1..n] are the
 * variable assignments or names.
 * @return Returns 0 on success, 1 on failure (invalid identifier).
 */
int	run_export(t_env_var *env_list, char **args)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (args[1] == NULL)
	{
		display_export(env_list);
		return (0);
	}
	while (args[i] != NULL)
	{
		if (export_variable(args[i], env_list) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
