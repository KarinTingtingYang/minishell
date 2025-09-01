/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:28:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 12:30:59 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is a valid identifier for environment variables.
 * A valid identifier starts with a letter or underscore, followed by letters,
 * digits, or underscores.
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
int	is_valid_identifier(const char *str)
{
	if (str == NULL || (!ft_isalpha(*str) && *str != '_'))
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
 * @brief Duplicates the environment variable linked list.
 *
 * This function creates a deep copy of the given linked list of
 * environment variables.
 *
 * @param env_list The head of the original environment variable linked list.
 * @return The head of the duplicated linked list, or NULL on failure.
 */
static void	swap_nodes(t_env_var *node1, t_env_var *node2)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = node1->key;
	temp_value = node1->value;
	node1->key = node2->key;
	node1->value = node2->value;
	node2->key = temp_key;
	node2->value = temp_value;
}

/**
 * @brief Performs a single pass of bubble sort on the linked list.
 *
 * This function iterates through the linked list and swaps adjacent nodes
 * if they are out of order. It returns 1 if any swaps were made, 0 otherwise.
 *
 * @param start The head of the environment variable linked list.
 * @return 1 if any swaps were made, 0 otherwise.
 */
int	perform_single_pass(t_env_var *start)
{
	t_env_var	*ptr1;
	size_t		len1;
	size_t		len2;
	size_t		n;
	int			swapped;

	swapped = 0;
	ptr1 = start;
	while (ptr1->next != NULL)
	{
		len1 = ft_strlen(ptr1->key);
		len2 = ft_strlen(ptr1->next->key);
		if (len1 > len2)
			n = len1;
		else
			n = len2;
		if (ft_strncmp(ptr1->key, ptr1->next->key, n) > 0)
		{
			swap_nodes(ptr1, ptr1->next);
			swapped = 1;
		}
		ptr1 = ptr1->next;
	}
	return (swapped);
}
