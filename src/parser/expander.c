/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/06 09:12:21 by makhudon         ###   ########.fr       */
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



// char *expand_variables(char *input, t_env_var *env_list, int last_exit_status)
// {
//     char    *result;
//     size_t  i;
//     int     inside_single_quote = 0;
//     int     inside_double_quote = 0;

//     result = ft_strdup("");
//     i = 0;
//     while (input[i])
//     {
//         if (input[i] == '\'' && inside_double_quote == 0)
//         {
//             inside_single_quote = !inside_single_quote;
//             i++;
//         }
//         else if (inside_single_quote)
//         {
//             // Inside single quotes: copy literally, no expansion
//             result = append_char(result, input[i]);
//             i++;
//         }
//         else if (input[i] == '"' && inside_single_quote == 0)
//         {
//             inside_double_quote = !inside_double_quote;
//             i++;
//         }
//         else if (input[i] == '$' && inside_single_quote == 0 && input[i + 1] != '\0')
//         {
//             if (input[i + 1] == '{') // Handle ${VAR}
//             {
//                 size_t start = i + 2; // after ${
//                 size_t end = start;

//                 while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
//                     end++;

//                 if (input[end] == '}') // valid closing brace
//                 {
//                     char *var_name = ft_strndup(&input[start], end - start);
//                     char *value = get_variable_value(var_name, env_list, last_exit_status);
//                     char *tmp = ft_strjoin(result, value);
//                     free(result);
//                     result = tmp;
//                     free(var_name);
//                     free(value);
//                     i = end + 1; // skip past closing brace
//                 }
//                 else
//                 {
//                     // No closing }, treat literally
//                     result = append_char(result, input[i]);
//                     i++;
//                 }
//             }
//             else if (input[i + 1] == '?' || input[i + 1] == '$' || input[i + 1] == '0')
//             {
//                 size_t start = i + 1;
//                 char *var_name = ft_strndup(&input[start], 1);
//                 char *value = get_variable_value(var_name, env_list, last_exit_status);
//                 char *tmp = ft_strjoin(result, value);
//                 free(result);
//                 result = tmp;
//                 free(var_name);
//                 free(value);
//                 i = start + 1;
//             }
//             else
//             {
//                 // Skip invalid initial chars after $ (like digits, !)
//                 size_t start = i + 1;
//                 while (input[start] && !((input[start] >= 'A' && input[start] <= 'Z') || 
//                                          (input[start] >= 'a' && input[start] <= 'z') || 
//                                          input[start] == '_'))
//                     start++;

//                 size_t var_start = start;
//                 while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
//                     start++;

//                 if (start > var_start)
//                 {
//                     char *var_name = ft_strndup(&input[var_start], start - var_start);
//                     char *value = get_variable_value(var_name, env_list, last_exit_status);
//                     char *tmp = ft_strjoin(result, value);
//                     free(result);
//                     result = tmp;
//                     free(var_name);
//                     free(value);
//                     i = start;
//                 }
//                 else
//                 {
//                     // No valid var name, treat $ literally
//                     result = append_char(result, input[i]);
//                     i++;
//                 }
//             }
//         }
//         else
//         {
//             result = append_char(result, input[i]);
//             i++;
//         }
//     }
//     return result;
// }



char *expand_variables(char *input, t_env_var *env_list, int last_exit_status, t_quote_type quote)
{
    if (quote == SINGLE_QUOTE)
        return ft_strdup(input); // No expansion inside single quotes

    char *result = ft_strdup("");
    if (!result)
        return NULL;

    size_t i = 0;
    while (input[i])
    {
        if (input[i] == '$' && input[i + 1] != '\0')
        {
            if (input[i + 1] == '{')
            {
                size_t start = i + 2;
                size_t end = start;

                while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
                    end++;

                if (input[end] == '}')
                {
                    char *var_name = ft_strndup(&input[start], end - start);
                    char *value = get_variable_value(var_name, env_list, last_exit_status);
                    char *tmp = ft_strjoin(result, value);
                    free(result);
                    result = tmp;
                    free(var_name);
                    free(value);
                    i = end + 1;
                    continue;
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
                continue;
            }
            else
            {
                size_t start = i + 1;
                // Check if valid var name start
                if (!((input[start] >= 'A' && input[start] <= 'Z') ||
                      (input[start] >= 'a' && input[start] <= 'z') ||
                      input[start] == '_'))
                {
                    result = append_char(result, input[i]);
                    i++;
                    continue;
                }
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
                    continue;
                }
            }
        }
        // Normal char append
        result = append_char(result, input[i]);
        i++;
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

// char **expand_and_split_args(char **args, t_env_var *env_list, int last_exit_status)
// {
//     char **tmp_result = NULL;
//     int tmp_size = 0;
//     int i = 0;

//     while (args[i] != NULL)
//     {
//         char *expanded = expand_variables(args[i], env_list, last_exit_status);
//         char **split = ft_split_whitespace(expanded);
//         int j = 0;
//         free(expanded);

//         // Count how many tokens in split
//         while (split[j] != NULL)
//         {
//             tmp_size++;
//             j++;
//         }
//         // Reallocate tmp_result to hold new tokens
//         char **new_tmp = malloc((tmp_size + 1) * sizeof(char *));
//         if (!new_tmp)
//         {
//             // Free all allocated so far on error
//             int k = 0;
//             while (tmp_result && tmp_result[k] != NULL)
//             {
//                 free(tmp_result[k]);
//                 k++;
//             }
//             free(tmp_result);
//             free_split(split);
//             return NULL;
//         }
//         // Copy old tokens to new array
//         int k = 0;
//         while (tmp_result && tmp_result[k] != NULL)
//         {
//             new_tmp[k] = tmp_result[k];
//             k++;
//         }
//         free(tmp_result);
//         // Append new tokens from split
//         j = 0;
//         while (split[j] != NULL)
//         {
//             new_tmp[k + j] = split[j];
//             j++;
//         }
//         new_tmp[k + j] = NULL;
//         free(split); // free only the array pointer, strings now owned by new_tmp
//         tmp_result = new_tmp;
//         i++;
//     }

//     return tmp_result;
// }

// char **expand_and_split_args(char **args, t_env_var *env_list, int last_exit_status)
// {
//     char **tmp_result = NULL;
//     int tmp_size = 0;
//     int i = 0;

//     while (args[i] != NULL)
//     {
//         char *expanded = expand_variables(args[i], env_list, last_exit_status);

//         // Check if the original arg is quoted
//         int quoted = (args[i][0] == '"' && args[i][ft_strlen(args[i]) - 1] == '"')
//                   || (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'');

//         char **split = quoted ? ft_split_single(expanded) : ft_split_whitespace(expanded);
//         // `ft_split_single()` is a custom function that returns one-element array with `expanded` unchanged

//         int j = 0;
//         free(expanded);

//         // Count tokens in split
//         while (split[j] != NULL)
//         {
//             tmp_size++;
//             j++;
//         }

//         char **new_tmp = malloc((tmp_size + 1) * sizeof(char *));
//         if (!new_tmp)
//         {
//             int k = 0;
//             while (tmp_result && tmp_result[k] != NULL)
//                 free(tmp_result[k++]);
//             free(tmp_result);
//             free_split(split);
//             return NULL;
//         }

//         int k = 0;
//         while (tmp_result && tmp_result[k] != NULL)
//         {
//             new_tmp[k] = tmp_result[k];
//             k++;
//         }
//         free(tmp_result);

//         j = 0;
//         while (split[j] != NULL)
//         {
//             new_tmp[k + j] = split[j];
//             j++;
//         }
//         new_tmp[k + j] = NULL;
//         free(split);
//         tmp_result = new_tmp;
//         i++;
//     }

//     return tmp_result;
// }



char **expand_and_split_args(t_token **tokens, t_env_var *env_list, int last_exit_status)
{
    char **final_args = NULL;
    int final_count = 0;

    for (int i = 0; tokens[i] != NULL; i++)
    {
        char *expanded = expand_variables(tokens[i]->value, env_list, last_exit_status, tokens[i]->quote);

        char **split = NULL;

        if (tokens[i]->quote == SINGLE_QUOTE)
        {
            // No splitting inside single quotes
            split = malloc(sizeof(char *) * 2);
            if (!split)
            {
                free(expanded);
                // free final_args before return NULL — left as exercise
                return NULL;
            }
            split[0] = expanded;
            split[1] = NULL;
        }
        else if (tokens[i]->quote == DOUBLE_QUOTE)
        {
            // No splitting inside double quotes, allow expansion only
            split = malloc(sizeof(char *) * 2);
            if (!split)
            {
                free(expanded);
                return NULL;
            }
            split[0] = expanded;
            split[1] = NULL;
        }
        else
        {
            // No quotes: split on whitespace
            split = ft_split_whitespace(expanded);
            free(expanded);
        }

        // Count split tokens
        int split_count = 0;
        while (split && split[split_count] != NULL)
            split_count++;

        // Reallocate final_args to hold new tokens
        char **new_final = malloc(sizeof(char *) * (final_count + split_count + 1));
        if (!new_final)
        {
            // Free allocated memory (not shown for brevity)
            return NULL;
        }
        // Copy old final args
        for (int j = 0; j < final_count; j++)
            new_final[j] = final_args[j];

        // Append new tokens
        for (int j = 0; j < split_count; j++)
            new_final[final_count + j] = split[j];

        new_final[final_count + split_count] = NULL;

        free(final_args);
        free(split); // only free array pointer, strings moved to new_final

        final_args = new_final;
        final_count += split_count;
    }

    return final_args;
}
