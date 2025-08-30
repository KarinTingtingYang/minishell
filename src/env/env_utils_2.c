/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 13:15:49 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 13:53:38 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Finds an environment variable by its key.
 *
 * This function searches the linked list of environment variables for a
 * variable with the specified key and returns a pointer to it.
 *
 * @param key The key of the environment variable to find.
 * @param env_list The head of the environment linked list.
 * @return A pointer to the found t_env_var, or NULL if not found.
 */
t_env_var	*find_env_var(const char *key, t_env_var *env_list)
{
	while (env_list != NULL)
	{
		if (ft_strncmp(env_list->key, key, ft_strlen(key) + 1) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

/**
 * @brief Creates a single "key=value" string from a t_env_var node.
 *
 * This function allocates and populates a new string representing a single
 * environment variable. It correctly handles the case where the value is NULL.
 *
 * @param env The environment variable node to convert.
 * @return A dynamically allocated string, or NULL on failure.
 */
static char	*create_env_entry(t_env_var *env)
{
	size_t	len_key;
	size_t	len_val;
	char	*entry;

	len_key = ft_strlen(env->key);
	len_val = 0;
	if (env->value != NULL)
		len_val = ft_strlen(env->value);
	entry = malloc(len_key + len_val + 2);
	if (entry == NULL)
		return (NULL);
	ft_strlcpy(entry, env->key, len_key + 1);
	entry[len_key] = '=';
	if (env->value != NULL)
		ft_strlcpy(entry + len_key + 1, env->value, len_val + 1);
	else
		entry[len_key + 1] = '\0';
	return (entry);
}

/**
 * @brief Calculates the length of the environment variable linked list.
 *
 * This function iterates through the linked list and counts the number
 * of nodes to determine the total length.
 *
 * @param env The head of the environment variable linked list.
 * @return The length of the linked list.
 */
static int	env_list_len(t_env_var *env)
{
	int	len;

	len = 0;
	while (env != NULL)
	{
		len++;
		env = env->next;
	}
	return (len);
}

/**
 * @brief Converts a linked list of environment variables to a string array.
 *
 * This function iterates through the environment list, converts each node
 * into a "key=value" string using a helper function, and populates a
 * null-terminated array of strings.
 *
 * @param env The head of the environment variable list.
 * @return A dynamically allocated array of strings, or NULL on failure.
 */
char	**env_list_to_array(t_env_var *env)
{
	int		len;
	char	**array;
	int		i;

	len = env_list_len(env);
	array = malloc(sizeof(char *) * (len + 1));
	i = 0;
	if (array == NULL)
		return (NULL);
	while (env != NULL)
	{
		array[i] = create_env_entry(env);
		if (array[i] == NULL)
		{
			while (i--)
				free(array[i]);
			free(array);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}
