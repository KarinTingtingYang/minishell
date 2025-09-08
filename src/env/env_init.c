/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:45:37 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/08 11:57:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Adds a new node to the end of the environment list.
 *
 * This function takes the head and tail of the list by reference and
 * adds a new node to the end. It correctly handles the case where the
 * list is empty.
 *
 * @param head A pointer to the head of the list.
 * @param tail A pointer to the tail of the list.
 * @param node The new node to add to the list.
 */
static void	add_node_to_list(t_env_var **head, t_env_var **tail,
													t_env_var *node)
{
	if (*head == NULL)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}

/**
 * @brief Creates a new t_env_var node with the given key and value.
 *
 * This function allocates memory for a new environment variable node and
 * assigns the provided key and value to it. It initializes the next pointer
 * to NULL. It handles memory allocation failures by returning NULL.
 *
 * @param key The key string for the new variable.
 * @param value The value string for the new variable.
 * @return A new, dynamically allocated t_env_var node, or NULL on failure.
 */
static t_env_var	*new_env_var(char *key, char *value)
{
	t_env_var	*node;

	node = malloc(sizeof(t_env_var));
	if (node == NULL)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

/**
 * @brief Parses an environment string and creates a new node.
 *
 * This function takes a single string from the environ array, splits it
 * into a key and a value, and creates a new t_env_var node. It handles
 * the case where a string does not contain an equal sign.
 *
 * @param env_string The string to parse (e.g., "PATH=/usr/bin").
 * @return A new t_env_var node, or NULL if parsing fails or string is invalid.
 */
static t_env_var	*parse_env_entry(const char *env_string)
{
	char	*equal_sign;
	size_t	key_len;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(env_string, '=');
	if (equal_sign == NULL)
		return (NULL);
	key_len = equal_sign - env_string;
	key = malloc(key_len + 1);
	if (key == NULL)
		return (NULL);
	ft_strlcpy(key, env_string, key_len + 1);
	value = ft_strdup(equal_sign + 1);
	if (value == NULL)
	{
		free(key);
		return (NULL);
	}
	return (new_env_var(key, value));
}

/**
 * @brief Initializes the environment variable list from a string array.
 *
 * This function takes the standard environ string array and converts it
 * into a linked list of t_env_var structs. It iterates through the array
 * and uses helper functions to handle parsing and adding nodes.
 *
 * @param environ The standard environment string array.
 * @return The head of the new linked list, or NULL on failure.
 */
t_env_var	*init_env(char **environ)
{
	t_env_var	*head;
	t_env_var	*tail;
	t_env_var	*node;
	int			i;

	if (environ == NULL || environ[0] == NULL)
		return (NULL);
	head = NULL;
	tail = NULL;
	i = 0;
	while (environ[i])
	{
		node = parse_env_entry(environ[i]);
		if (node)
			add_node_to_list(&head, &tail, node);
		i++;
	}
	return (head);
}

/**
 * @brief Frees the entire environment variable linked list.
 *
 * This function iterates through the linked list and frees each node's
 * key, value, and the node itself to prevent memory leaks.
 *
 * @param env The head of the environment variable linked list to free.
 */
void	free_env(t_env_var *env)
{
	t_env_var	*tmp;

	while (env != NULL)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}
