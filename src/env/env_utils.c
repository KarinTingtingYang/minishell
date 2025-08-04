/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:47:23 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/04 09:56:56 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Finds an environment variable in the linked list.
 *
 * @param key The key of the variable to find.
 * @param env_list The head of the environment list.
 * @return A pointer to the t_env_var node if found, otherwise NULL.
 */
t_env_var	*find_env_var(const char *key, t_env_var *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, ft_strlen(key) + 1) == 0)
			return (env_list);
		env_list = env_list->next;
	}
	return (NULL);
}

/**
 * @brief Adds a new environment variable to the end of the list.
 *
 * @param key The key of the new variable (will be duplicated).
 * @param value The value of the new variable (will be duplicated).
 * @param env_list The head of the environment list.
 */
void	add_env_var(const char *key, const char *value, t_env_var *env_list)
{
	t_env_var	*new_var;
	t_env_var	*current;

	new_var = malloc(sizeof(t_env_var));
	if (!new_var)
		error_exit("malloc");
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = NULL;
	if (!new_var->key || !new_var->value)
		error_exit("malloc");
	if (!env_list)
		env_list = new_var;
	else
	{
		current = env_list;
		while (current->next)
			current = current->next;
		current->next = new_var;
	}
}


/*
 * @brief Counts the number of environment variables in the linked list.
 *
 * @param env The head of the environment list.
 * @return The number of environment variables.
 */
int env_list_len(t_env_var *env)
{
    int len = 0;
    while (env)
    {
        len++;
        env = env->next;
    }
    return len;
}

/**
 * @brief Converts the environment linked list to a char** array.
 *
 * @param env The head of the environment list.
 * @return A dynamically allocated array of strings representing the environment
 * variables, or NULL on failure.
 */
char **env_list_to_array(t_env_var *env)
{
    int len = env_list_len(env);
    char **arr = malloc(sizeof(char *) * (len + 1));
    int i = 0;

    if (arr == NULL)
        return NULL;

    while (env)
    {
        size_t len_key = strlen(env->key);
        size_t len_val = 0;
        char *entry;
        if (env->value)
            len_val = ft_strlen(env->value);
        entry = malloc(len_key + len_val + 2);
        if (entry == NULL)
        {
            while (i--)
                free(arr[i]);
            free(arr);
            return NULL;
        }
        ft_strlcpy(entry, env->key, len_key + 1);
        entry[len_key] = '=';
        if (env->value)
            ft_strlcpy(entry + len_key + 1, env->value, len_val + 1);
        else
            entry[len_key + 1] = '\0';
        arr[i++] = entry;
        env = env->next;
    }
    arr[i] = NULL;
    return arr;
}

/**
 * @brief Creates a deep copy of the environment linked list.
 * @param env_list The list to duplicate.
 * @return The head of the new, duplicated list.
 */
t_env_var	*duplicate_env_list(t_env_var *env_list)
{
	t_env_var	*new_head;
	t_env_var	*new_tail;
	t_env_var	*temp;

	new_head = NULL;
	new_tail = NULL;
	while (env_list)
	{
		temp = malloc(sizeof(t_env_var));
		if (!temp)
			error_exit("malloc");
		temp->key = ft_strdup(env_list->key);
		temp->value = ft_strdup(env_list->value);
		if (!temp->key || !temp->value)
			error_exit("malloc");
		temp->next = NULL;
		if (new_head == NULL)
		{
			new_head = temp;
			new_tail = temp;
		}
		else
		{
			new_tail->next = temp;
			new_tail = temp;
		}
		env_list = env_list->next;
	}
	return (new_head);
}