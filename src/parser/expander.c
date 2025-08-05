/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/05 12:14:55 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Finds the value of an environment variable by its name.
 * 
 * This function searches for the given variable name in the linked list
 * of environment variables. If found, it returns a dynamically allocated
 * string containing the variable's value. If the variable is not found,
 * it returns an empty string.
 * @param var_name The name of the environment variable to find.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A dynamically allocated string with the variable's value, or
 *         an empty string if not found.
 */
static char	*get_variable_value(const char *var_name, t_env_var *env_list, int last_exit_status)
{
    t_env_var *env_var;

    if (ft_strncmp(var_name, "?", 2) == 0)
        return (ft_itoa(last_exit_status));
    if (ft_strncmp(var_name, "$", 2) == 0)
        return (ft_itoa(getpid()));
    if (ft_strncmp(var_name, "0", 2) == 0)
        return (ft_strdup("minishell"));
    env_var = find_env_var(var_name, env_list);
    if (env_var != NULL)
        return (ft_strdup(env_var->value));
    return (ft_strdup(""));
}

/**
 * @brief Expands variables in a string based on the environment variables.
 * 
 * This function processes the input string, replacing variable references
 * (e.g., $VAR) with their corresponding values from the environment list.
 * It handles single and double quotes to determine when to expand variables.
 * @param input The input string containing variable references.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A new string with variables expanded, or NULL on failure.
 */
// char *expand_variables(char *input, t_env_var *env_list, int last_exit_status)
// {
// 	char	*result;
// 	size_t	i;
// 	int		inside_single_quote;
// 	int		inside_double_quote;

// 	result = ft_strdup("");
// 	i = 0;
// 	inside_single_quote = 0;
// 	inside_double_quote = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '\'' && inside_double_quote == 0)
// 			inside_single_quote = !inside_single_quote;
// 		else if (input[i] == '"' && inside_single_quote == 0)
// 			inside_double_quote = !inside_double_quote;
// 		// else if (input[i] == '$' && inside_single_quote == 0 && input[i + 1] != '\0')
// 		// {
// 		// 	size_t start = i + 1;

// 		// 	if (input[start] == '?' || input[start] == '$' || input[start] == '0')
// 		// 		i = start + 1;
// 		// 	else
// 		// 	{
// 		// 		i = start;
// 		// 		while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
// 		// 			i++;
// 		// 	}
// 		// 	char *var_name = ft_strndup(&input[start], i - start);
// 		// 	char *value = get_variable_value(var_name, env_list, last_exit_status);
// 		// 	char *tmp = ft_strjoin(result, value);
// 		// 	free(result);
// 		// 	result = tmp;
// 		// 	free(var_name);
// 		// 	free(value);
// 		// 	continue;
// 		// }
// 		else if (input[i] == '$' && inside_single_quote == 0 && input[i + 1] != '\0')
// 		{
// 			size_t start = i + 1;
    
// 			if (input[start] == '?' || input[start] == '$' || input[start] == '0')
// 				i = start + 1;
// 			else
// 			{
// 				while (input[start] && !((input[start] >= 'A' && input[start] <= 'Z') || (input[start] >= 'a' && input[start] <= 'z') || input[start] == '_'))
// 					start++;
// 				i = start;
// 				while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
// 					i++;
// 			}
// 			if (i > start)
// 			{
// 				char *var_name = ft_strndup(&input[start], i - start);
// 				char *value = get_variable_value(var_name, env_list, last_exit_status);
// 				char *tmp = ft_strjoin(result, value);
// 				free(result);
// 				result = tmp;
// 				free(var_name);
// 				free(value);
// 			}
// 			else
// 			{
// 				result = append_char(result, '$');
// 			}
// 			continue;
// 		}
// 		else
// 			result = append_char(result, input[i]);
// 		i++;
// 	}
// 	return (result);
// }



char *expand_variables(char *input, t_env_var *env_list, int last_exit_status)
{
    char    *result;
    size_t  i;
    int     inside_single_quote = 0;
    int     inside_double_quote = 0;

    result = ft_strdup("");
    i = 0;
    while (input[i])
    {
        if (input[i] == '\'' && inside_double_quote == 0)
        {
            inside_single_quote = !inside_single_quote;
            i++;
        }
        else if (inside_single_quote)
        {
            // Inside single quotes: copy literally, no expansion
            result = append_char(result, input[i]);
            i++;
        }
        else if (input[i] == '"' && inside_single_quote == 0)
        {
            inside_double_quote = !inside_double_quote;
            i++;
        }
        else if (input[i] == '$' && inside_single_quote == 0 && input[i + 1] != '\0')
        {
            if (input[i + 1] == '{') // Handle ${VAR}
            {
                size_t start = i + 2; // after ${
                size_t end = start;

                while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
                    end++;

                if (input[end] == '}') // valid closing brace
                {
                    char *var_name = ft_strndup(&input[start], end - start);
                    char *value = get_variable_value(var_name, env_list, last_exit_status);
                    char *tmp = ft_strjoin(result, value);
                    free(result);
                    result = tmp;
                    free(var_name);
                    free(value);
                    i = end + 1; // skip past closing brace
                }
                else
                {
                    // No closing }, treat literally
                    result = append_char(result, input[i]);
                    i++;
                }
            }
            else if (input[i + 1] == '?' || input[i + 1] == '$' || input[i + 1] == '0')
            {
                size_t start = i + 1;
                char *var_name = ft_strndup(&input[start], 1);
                char *value = get_variable_value(var_name, env_list, last_exit_status);
                char *tmp = ft_strjoin(result, value);
                free(result);
                result = tmp;
                free(var_name);
                free(value);
                i = start + 1;
            }
            else
            {
                // Skip invalid initial chars after $ (like digits, !)
                size_t start = i + 1;
                while (input[start] && !((input[start] >= 'A' && input[start] <= 'Z') || 
                                         (input[start] >= 'a' && input[start] <= 'z') || 
                                         input[start] == '_'))
                    start++;

                size_t var_start = start;
                while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
                    start++;

                if (start > var_start)
                {
                    char *var_name = ft_strndup(&input[var_start], start - var_start);
                    char *value = get_variable_value(var_name, env_list, last_exit_status);
                    char *tmp = ft_strjoin(result, value);
                    free(result);
                    result = tmp;
                    free(var_name);
                    free(value);
                    i = start;
                }
                else
                {
                    // No valid var name, treat $ literally
                    result = append_char(result, input[i]);
                    i++;
                }
            }
        }
        else
        {
            result = append_char(result, input[i]);
            i++;
        }
    }
    return result;
}





// void expand_args(char **args, t_env_var *env_list, int last_exit_status)
// {
//     int i = 0;
//     char *expanded;

//     while (args[i])
//     {
//         expanded = expand_variables(args[i], env_list, last_exit_status);
//         free(args[i]);
//         args[i] = expanded;
//         i++;
//     }
// }

char **expand_and_split_args(char **args, t_env_var *env_list, int last_exit_status)
{
    char **tmp_result = NULL;
    int tmp_size = 0;
    int i = 0;

    while (args[i] != NULL)
    {
        char *expanded = expand_variables(args[i], env_list, last_exit_status);
        char **split = ft_split_whitespace(expanded);
        int j = 0;
        free(expanded);

        // Count how many tokens in split
        while (split[j] != NULL)
        {
            tmp_size++;
            j++;
        }
        // Reallocate tmp_result to hold new tokens
        char **new_tmp = malloc((tmp_size + 1) * sizeof(char *));
        if (!new_tmp)
        {
            // Free all allocated so far on error
            int k = 0;
            while (tmp_result && tmp_result[k] != NULL)
            {
                free(tmp_result[k]);
                k++;
            }
            free(tmp_result);
            free_split(split);
            return NULL;
        }
        // Copy old tokens to new array
        int k = 0;
        while (tmp_result && tmp_result[k] != NULL)
        {
            new_tmp[k] = tmp_result[k];
            k++;
        }
        free(tmp_result);
        // Append new tokens from split
        j = 0;
        while (split[j] != NULL)
        {
            new_tmp[k + j] = split[j];
            j++;
        }
        new_tmp[k + j] = NULL;
        free(split); // free only the array pointer, strings now owned by new_tmp
        tmp_result = new_tmp;
        i++;
    }

    return tmp_result;
}
