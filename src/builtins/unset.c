/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariahudonogova <mariahudonogova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 08:47:50 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/28 23:20:28 by mariahudono      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Removes an environment variable from the linked list.
 *
 * This function searches for the environment variable with the specified key
 * and removes it from the linked list if found.
 * @param env_list Pointer to the head of the environment linked list.
 * @param key The key of the environment variable to remove.
 */
static void remove_env_var(t_env_var **env_list, const char *key)
{
    t_env_var *current_var;
    t_env_var *previous_var;

    current_var = *env_list;
    previous_var = NULL;
    while (current_var != NULL)
    {
        if (ft_strncmp(current_var->key, key, ft_strlen(key) + 1) == 0)
        {
            if (previous_var != NULL)
                previous_var->next = current_var->next;
            else
                *env_list = current_var->next;
            free(current_var->key);
            free(current_var->value);
            free(current_var);
            return ; // Exit after removing the variable
        }
        previous_var = current_var;
        current_var = current_var->next;
    }
}

/**
 * @brief Checks if a string is a valid variable name for unset.
 *
 * A valid variable name starts with a letter or underscore, followed by
 * letters, digits, or underscores.
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
static int is_valid_varname(const char *str)
{
    int i;
	
	i = 0;
    if (str == NULL || !(ft_isalpha(str[0]) || str[0] == '_'))
        return (0);
    while (str[i] != '\0')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1); // Return 1 if valid
}

/**
 * @brief Unsets environment variables by removing them from the linked list.
 *
 * @param env_list Pointer to the head of the environment linked list.
 * @param args Array of arguments, where each argument is a variable name to unset.
 * @return Returns 0 on success, or 1 if an invalid identifier is encountered.
 */
int run_unset(t_env_var **env_list, char **args)
{
    int i;
	int exit_status; //Debug: Track exit status
	
	i = 1;
	exit_status = 0;
    while (args[i] != NULL)
    {
		if (ft_strchr(args[i], '=') != NULL) //DEBUG
        {
            i++;
            continue ;
        }
        if (!is_valid_varname(args[i]))
        {	
			// ft_putstr_fd("unset: `", STDERR_FILENO); // DEBUG: Print error if variable name is invalid
			// ft_putstr_fd(args[i], STDERR_FILENO); // DEBUG: Print the invalid variable name
			// ft_putendl_fd("`: not a valid identifier", STDERR_FILENO); // DEBUG: Print the error message
			// ft_error("unset", "not a valid identifier");
			// exit_status = 1;
			i++;
			continue;
		}
        // else
        remove_env_var(env_list, args[i]);
        i++;
    }
    return (exit_status); // Debug: Return exit status
}
