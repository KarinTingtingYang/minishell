/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:03:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/16 14:07:37 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// REPLACED QSORT WITH BUBBLE SORT AS QSORT IS NOT ALLOWED IN THIS PROJECT

/**
 * @brief Checks if a string is a valid identifier for an environment variable.
 * (Starts with a letter or underscore, followed by letters, digits, or
 * underscores).
 *
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}


/**
 * @brief Sorts an environment linked list alphabetically using bubble sort
 * with ft_strncmp.
 * @param start The head of the list to sort.
 */
static void	bubble_sort_env_list(t_env_var *start)
{
	int			swapped;
	t_env_var	*ptr1;
	char		*temp_key;
	char		*temp_value;
	size_t		len1;
	size_t		len2;
	size_t		n;

	if (start == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = start;
		while (ptr1->next != NULL)
		{
			len1 = ft_strlen(ptr1->key);
			len2 = ft_strlen(ptr1->next->key);
			n = (len1 > len2) ? len1 : len2;
			if (ft_strncmp(ptr1->key, ptr1->next->key, n) > 0)
			{
				// Swap the data of the two nodes
				temp_key = ptr1->key;
				temp_value = ptr1->value;
				ptr1->key = ptr1->next->key;
				ptr1->value = ptr1->next->value;
				ptr1->next->key = temp_key;
				ptr1->next->value = temp_value;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
	}
}


/**
 * @brief Duplicates, sorts, and prints the environment list for the 'export'
 * command.
 * @param env_list The original environment list.
 */
static void	display_export(t_env_var *env_list)
{
	t_env_var	*sorted_list;
	t_env_var	*current;

	// 1. Duplicate the list so we don't alter the original
	sorted_list = duplicate_env_list(env_list);

	// if (sorted_list == NULL) // DEBUG: Check if duplication failed
    //     return (1);
		
	// 2. Sort the duplicated list
	bubble_sort_env_list(sorted_list);

	// 3. Print the sorted list in the required format
	current = sorted_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->key, STDOUT_FILENO);
		if (current->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}

	// 4. Free the duplicated list to prevent memory leaks
	free_env(sorted_list);
}

static int	export_variable(const char *arg, t_env_var *env_list)
{
	char		*key;
	char		*value;
	char		*equal_sign;
	t_env_var	*existing_var;
	int			append;

	append = 0;
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		if (!is_valid_identifier(arg))
		{
			ft_error("export", "not a valid identifier");
			return (1);
		}
		existing_var = find_env_var(arg, env_list);
		if (!existing_var)
			add_env_var(ft_strdup(arg), ft_strdup(""), env_list);
		return (0);
	}
	// Detect '+=' syntax
	if (*(equal_sign - 1) == '+') // DEBUGGING
	{
		append = 1;
		key = ft_substr(arg, 0, equal_sign - arg - 1); // Exclude '+'
	}
	else
		key = ft_substr(arg, 0, equal_sign - arg);

	if (!is_valid_identifier(key))
	{
		ft_error("export", "not a valid identifier");
		free(key);
		return (1);
	}

	value = ft_strdup(equal_sign + 1);

	// Strip surrounding quotes if both first and last chars are matching quotes
	// if (value && ((value[0] == '"' || value[0] == '\'') && value[ft_strlen(value) - 1] == value[0]))
	// {
	// 	char *unquoted = ft_substr(value, 1, ft_strlen(value) - 2);
	// 	free(value);
	// 	value = unquoted;
	// }
	size_t len = ft_strlen(value);
	if (value && len >= 2
		&& ((value[0] == '\'' && value[len - 1] == '\'')
			|| (value[0] == '"' && value[len - 1] == '"')))
	{
		char *unquoted = ft_substr(value, 1, len - 2);
		free(value);
		value = unquoted;
	}

	existing_var = find_env_var(key, env_list);
	if (existing_var)
	{
		if (append && existing_var->value)
		{
			char *new_val = ft_strjoin(existing_var->value, value);
			free(existing_var->value);
			existing_var->value = new_val;
		}
		else
		{
			free(existing_var->value);
			existing_var->value = value;
		}
	}
	else
	{
		add_env_var(key, value, env_list);
		if (!append)
			free(value); // Only free if we added normally (value copied in add_env_var)
	}

	free(key);
	return (0);
}

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
	while (args[i])
	{
		if (export_variable(args[i], env_list) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
