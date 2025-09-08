/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:15:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 12:18:50 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Frees a NULL-terminated array of strings.
 * 
 * This function iterates through the array, freeing each individual
 * string, and then frees the array itself.
 * @param array The NULL-terminated array of strings to free.
 */
void	free_split(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}

/**
 * @brief Skips leading spaces and tabs in a string.
 * 
 * This function advances the pointer to the first non-space,
 * non-tab character in the string.
 * @param s A pointer to the string pointer to modify.
 */
void	skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
		(*s)++;
}

/**
 * @brief Adds a new environment variable node to the linked list.
 *
 * This function handles two main cases: creating the very first node
 * if the list is empty, or appending a new node to the end of an existing list.
 * It ensures proper memory management by duplicating the key and value strings,
 * preventing potential memory leaks.
 *
 * @param head A pointer to the head of the linked list.
 * @param key The key (name) of the environment variable (e.g., "PATH").
 * @param value The value of the environment variable. Can be NULL.
 */
static void	add_first_or_append(t_env_var **head, const char *key,
										const char *value)
{
	t_env_var	*new_node;

	if (*head == NULL)
	{
		new_node = (t_env_var *)malloc(sizeof(*new_node));
		if (new_node == NULL)
			ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
		new_node->key = ft_strdup(key);
		if (value != NULL)
			new_node->value = ft_strdup(value);
		else
			new_node->value = NULL;
		new_node->next = NULL;
		*head = new_node;
		return ;
	}
	if (value != NULL)
		add_env_var(ft_strdup(key), ft_strdup(value), *head);
	else
		add_env_var(ft_strdup(key), NULL, *head);
}

/**
 * @brief Initializes the environment list if it is currently empty.
 *
 * This function serves as a safety net for the shell. It checks if the
 * environment variable list has any entries. If the list is empty,
 * it "bootstraps" a minimal set of essential variables. This ensures the shell
 * has a basic functional environment to operate in, even when it is not
 * launched from an existing one. The function exits immediately if the
 * environment list is already populated, preventing any unintended
 * modifications.
 *
 * @param env_list A pointer to the head of the environment variable linked list.
 * This is where new variables will be added.
 */
void	bootstrap_env_if_empty(t_env_var **env_list)
{
	char	cwd[PATH_MAX];

	if (env_list == NULL)
		return ;
	if (*env_list != NULL)
		return ;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		add_first_or_append(env_list, "PWD", cwd);
	add_first_or_append(env_list, "SHLVL", "1");
	add_first_or_append(env_list, "OLDPWD", NULL);
}
