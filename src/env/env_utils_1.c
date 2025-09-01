/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:47:23 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 13:19:34 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Creates a new t_env_var node with the given key and value.
 *
 * This function allocates memory for a new environment variable node and
 * duplicates the key and value strings into it. It handles memory allocation
 * failures by exiting the program.
 *
 * @param key The key string for the new variable.
 * @param value The value string for the new variable, can be NULL.
 * @return A new, dynamically allocated t_env_var node.
 */
static t_env_var	*create_new_env_node(const char *key, const char *value)
{
	t_env_var	*new_var;

	new_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (new_var == NULL)
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	new_var->key = ft_strdup(key);
	if (new_var->key == NULL)
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	if (value != NULL)
	{
		new_var->value = ft_strdup(value);
		if (new_var->value == NULL)
			ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	}
	else
		new_var->value = NULL;
	new_var->next = NULL;
	return (new_var);
}

/**
 * @brief Adds a new environment variable to the end of the list.
 *
 * This function first creates a new node using a helper function and then
 * traverses the existing linked list to add the new node at the end.
 * It handles the case where the list is initially empty.
 *
 * @param key The key of the variable to add.
 * @param value The value of the variable to add, can be NULL.
 * @param env_list The head of the environment variable linked list.
 */
void	add_env_var(const char *key, const char *value, t_env_var *env_list)
{
	t_env_var	*new_var;
	t_env_var	*cur;

	new_var = create_new_env_node(key, value);
	if (env_list == NULL)
		env_list = new_var;
	else
	{
		cur = env_list;
		while (cur->next != NULL)
			cur = cur->next;
		cur->next = new_var;
	}
}

/**
 * @brief Links a new node to the end of a linked list.
 *
 * This function handles the logic for linking a new node to the head or
 * tail of an existing list. It's used as a helper function to keep the
 * main duplication logic clean.
 *
 * @param new_head A pointer to the head of the new list.
 * @param new_tail A pointer to the tail of the new list.
 * @param node_to_add The new node to link to the list.
 */
static void	link_new_node(t_env_var **new_head, t_env_var **new_tail,
													t_env_var *node_to_add)
{
	if (*new_head == NULL)
		*new_head = node_to_add;
	else
		(*new_tail)->next = node_to_add;
	*new_tail = node_to_add;
}

/**
 * @brief Duplicates a single environment variable node.
 *
 * This function allocates a new node and duplicates its key and value.
 * It handles all memory allocation and error checking for a single node.
 *
 * @param source The source node to duplicate.
 * @return A pointer to the newly created node, or NULL on failure.
 */
static t_env_var	*create_duplicated_node(t_env_var *source)
{
	t_env_var	*node;

	node = malloc(sizeof(*node));
	if (node == NULL)
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	node->key = ft_strdup(source->key);
	if (node->key == NULL)
	{
		free(node);
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
	}
	if (source->value != NULL)
	{
		node->value = ft_strdup(source->value);
		if (node->value == NULL)
		{
			free(node->key);
			free(node);
			ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
		}
	}
	else
		node->value = NULL;
	node->next = NULL;
	return (node);
}

/**
 * @brief Creates a deep copy of an environment variable linked list.
 *
 * This is the main function that orchestrates the duplication process.
 * It iterates through the original list and uses helper functions to
 * create and link new nodes to the duplicated list.
 *
 * @param env_list The head of the environment variable linked list.
 * @return A pointer to the head of the newly created list.
 */
t_env_var	*duplicate_env_list(t_env_var *env_list)
{
	t_env_var	*new_head;
	t_env_var	*new_tail;
	t_env_var	*node;

	new_head = NULL;
	new_tail = NULL;
	while (env_list != NULL)
	{
		node = create_duplicated_node(env_list);
		link_new_node(&new_head, &new_tail, node);
		env_list = env_list->next;
	}
	return (new_head);
}
