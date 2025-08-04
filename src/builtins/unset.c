/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:47:50 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/04 09:25:40 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_valid_varname(const char *str)
{
    int i = 0;

    if (!str || !(ft_isalpha(str[0]) || str[0] == '_'))
        return 0;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

static void remove_env_var(t_env_var **env_list, const char *key)
{
    t_env_var *curr = *env_list;
    t_env_var *prev = NULL;

    while (curr)
    {
        if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
        {
            if (prev)
                prev->next = curr->next;
            else
                *env_list = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int run_unset(t_env_var **env_list, char **args)
{
    int i = 1;

    while (args[i])
    {
        if (!is_valid_varname(args[i]))
        {	
			ft_putstr_fd("unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("`: not a valid identifier", STDERR_FILENO);
		}
        else
            remove_env_var(env_list, args[i]);
        i++;
    }
    return 0;
}
