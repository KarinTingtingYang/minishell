/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 10:15:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/09 12:48:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

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
 * @brief Cleans up resources before exiting the shell.
 * Frees environment list and clears readline history.
 * @param process_data Pointer to the process data structure.
 * @return void
 */
void	shutdown_shell(t_process_data *process_data)
{
	get_next_line_cleanup();
	rl_clear_history();
	if (process_data && process_data->env_list)
	{
		free_env(process_data->env_list);
		process_data->env_list = NULL;
	}
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
