/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 13:03:36 by makhudon      #+#    #+#                 */
/*   Updated: 2025/07/31 10:05:11 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int env_list_len(t_env_var *env)
{
    int len = 0;
    while (env)
    {
        len++;
        env = env->next;
    }
    return len;
}

// DEBUG FIX EXPORT :) made this function visible to other files
char **env_list_to_array(t_env_var *env)
{
    int len = env_list_len(env);
    char **arr = malloc(sizeof(char *) * (len + 1));
    int i = 0;

    while (env)
    {
        size_t len_key = strlen(env->key);
        size_t len_val = env->value ? strlen(env->value) : 0;
        char *entry = malloc(len_key + len_val + 2); // for '=' and '\0'
        if (!entry)
        {
            while (i--)
                free(arr[i]);
            free(arr);
            return NULL;
        }
        if (env->value)
            sprintf(entry, "%s=%s", env->key, env->value);
        else
            sprintf(entry, "%s=", env->key);
        arr[i++] = entry;
        env = env->next;
    }
    arr[i] = NULL;
    return arr;
}


static int cmp_env(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;

    // Compare until '=' or end of string
    int i = 0;
    while (str1[i] && str2[i] && str1[i] != '=' && str2[i] != '=')
    {
        if (str1[i] != str2[i])
            return ((unsigned char)str1[i] - (unsigned char)str2[i]);
        i++;
    }
    if (str1[i] == '=' && str2[i] == '=')
        return 0;
    if (str1[i] == '=')
        return -1;
    if (str2[i] == '=')
        return 1;
    return 0;
}


int run_export(t_env_var *env_list, char **args)
{
    if (args[1] != NULL)
    {
        // For now, ignore export with arguments
        return 1;
    }

    char **env_arr = env_list_to_array(env_list);
    if (!env_arr)
        return 1;

    int len = 0;
    while (env_arr[len])
        len++;

    qsort(env_arr, len, sizeof(char *), cmp_env);

    for (int i = 0; i < len; i++)
    {
        char *equal_sign = strchr(env_arr[i], '=');
        if (equal_sign)
        {
            *equal_sign = '\0';
            printf("declare -x %s=\"%s\"\n", env_arr[i], equal_sign + 1);
            *equal_sign = '=';
        }
        else
        {
            printf("declare -x %s\n", env_arr[i]);
        }
        free(env_arr[i]);
    }
    free(env_arr);
    return 0;
}
