/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 11:45:37 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/13 15:14:05 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_env_var *new_env_var(char *key, char *value)
{
    t_env_var *node;

    node = malloc(sizeof(t_env_var));
    if (!node)
        return (NULL);
    node->key = key;
    node->value = value;
    node->next = NULL;
    return (node);
}

t_env_var *init_env(char **environ)
{
    t_env_var *head = NULL;
    t_env_var *tail = NULL;
    int i = 0;

	if (!environ)
    {
        printf("DEBUG: init_env received a NULL environment array!\n");
        return (NULL);
    }
	if (!environ[0]) {
        printf("init_env: envp[0] is NULL (empty environment)\n");
        return NULL;
    }
    while (environ[i])
    {
		printf("DEBUG: init_env processing string [%d]: \"%s\"\n", i, environ[i]);
        char *equal = ft_strchr(environ[i], '=');
        if (!equal)
        {
            i++;
            continue;
        }
        size_t key_len = equal - environ[i];
        char *key = malloc(key_len + 1);
        if (!key)
		{
			printf("DEBUG: init_env FAILED: malloc for key failed.\n");
			return (NULL); // handle cleanup in real code
		}
            
		ft_strlcpy(key, environ[i], key_len + 1); // DEBUG FIX EXPORT! :)
        key[key_len] = '\0';

        char *value = ft_strdup(equal + 1);
        if (!value)
        {
			printf("DEBUG: init_env FAILED: strdup for value failed.\n");
            free(key);
            return (NULL);
        }
        t_env_var *node = new_env_var(key, value);
        if (!node)
        {
			printf("DEBUG: init_env FAILED: new_env_var (malloc for node) failed.\n");
            free(key);
            free(value);
            return (NULL);
        }
        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
        i++;
    }
    return (head);
}

void free_env(t_env_var *env)
{
    t_env_var *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}
