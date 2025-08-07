/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 15:49:56 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a token is an export assignment.
 * 
 * This function checks if the token's value contains an '=' character,
 * indicating that it is an export assignment. It returns true if it is
 * an export assignment, false otherwise.
 * @param token The token to check.
 * @return 1 if the token is an export assignment, 0 otherwise.
 */
static int is_export_assignment(t_token *token)
{
	return (ft_strchr(token->value, '=') != NULL);
}

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

// char *expand_variables(char *input, t_env_var *env_list, int last_exit_status, t_quote_type quote)
// {
//     if (quote == SINGLE_QUOTE)
//         return ft_strdup(input); // No expansion inside single quotes
//     char *result = ft_strdup("");
//     if (result == NULL)
//         return NULL;
//     size_t i = 0;
//     while (input[i] != '\0')
//     {
//         if (input[i] == '$' && input[i + 1] != '\0')
//         {
//             if (input[i + 1] == '{')
//             {
//                 size_t start = i + 2;
//                 size_t end = start;

//                 while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
//                     end++;
//                 if (input[end] == '}')
//                 {
//                     char *var_name = ft_strndup(&input[start], end - start);
//                     char *value = get_variable_value(var_name, env_list, last_exit_status);
//                     char *tmp = ft_strjoin(result, value);
//                     free(result);
//                     result = tmp;
//                     free(var_name);
//                     free(value);
//                     i = end + 1;
//                     continue;
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
//                 continue;
//             }
//             else
//             {
//                 size_t start = i + 1;
//                 // Check if valid var name start
//                 if (!((input[start] >= 'A' && input[start] <= 'Z') ||
//                       (input[start] >= 'a' && input[start] <= 'z') ||
//                       input[start] == '_'))
//                 {
//                     result = append_char(result, input[i]);
//                     i++;
//                     continue;
//                 }
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
//                     continue;
//                 }
//             }
//         }
//         // Normal char append
//         result = append_char(result, input[i]);
//         i++;
//     }
//     return result;
// }


/**
 * @brief Expands a variable enclosed in braces (e.g., ${
 * This function processes the input string to find a variable
 * enclosed in braces (e.g., ${VAR_NAME}) and replaces it with the
 * variable's value from the environment list.
 * @param input The input string containing the variable.
 * @param i The current index in the input string.
 * @param result Pointer to the string where the result will be stored.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return The updated index after processing the variable.
 * If the variable is found, it appends its value to the result.
 * If the variable is not found or the syntax is incorrect,
 * it returns the current index without modifying the result.
 */
static int	expand_braced_variable(const char *input, size_t i,
				char **result, t_env_var *env_list, int last_exit_status)
{
	size_t	start = i + 2;
	size_t	end = start;

	while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
		end++;
	if (input[end] == '}')
	{
		char	*var_name = ft_strndup(&input[start], end - start);
		char	*value = get_variable_value(var_name, env_list, last_exit_status);
		char	*tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(var_name);
		free(value);
		return (end + 1);
	}
	return (i); // No valid closing brace, treat as normal text
}

/**
 * @brief Appends a character to a string, reallocating memory if necessary.
 * 
 * This function appends a single character to the end of a string, reallocating
 * memory if needed. It returns the new string with the appended character.
 * @param str The original string to append to.
 * @param c The character to append.
 * @return A new string with the character appended, or NULL on failure.
 */
static int	expand_simple_variable(const char *input, size_t i,
				char **result, t_env_var *env_list, int last_exit_status)
{
	size_t	start = i + 1;

	if (input[start] == '?' || input[start] == '$' || input[start] == '0')
	{
		char	*var_name = ft_strndup(&input[start], 1);
		char	*value = get_variable_value(var_name, env_list, last_exit_status);
		char	*tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(var_name);
		free(value);
		return (start + 1);
	}
	else if ((input[start] >= 'A' && input[start] <= 'Z') ||
			 (input[start] >= 'a' && input[start] <= 'z') ||
			 input[start] == '_')
	{
		size_t	var_start = start;
		while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
			start++;
		char	*var_name = ft_strndup(&input[var_start], start - var_start);
		char	*value = get_variable_value(var_name, env_list, last_exit_status);
		char	*tmp = ft_strjoin(*result, value);
		free(*result);
		*result = tmp;
		free(var_name);
		free(value);
		return (start);
	}
	else
	{
		*result = append_char(*result, input[i]);
		return (i + 1);
	}
}

/**
 * @brief Expands environment variables in a string based on the quote type.
 * 
 * This function processes the input string, expanding environment variables
 * and handling quoted text. It returns a new string with the expanded content.
 * @param input The input string to process.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @param quote The type of quote enclosing the input (if any).
 * @return A dynamically allocated string with the expanded content, or NULL on failure.
 */
char	*expand_variables(char *input, t_env_var *env_list, int last_exit_status, t_quote_type quote)
{
	if (quote == SINGLE_QUOTE)
		return (ft_strdup(input));
	char	*result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	size_t	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '$' && input[i + 1])
		{
			if (input[i + 1] == '{')
				i = expand_braced_variable(input, i, &result, env_list, last_exit_status);
			else
				i = expand_simple_variable(input, i, &result, env_list, last_exit_status);
		}
		else
		{
			result = append_char(result, input[i]);
			i++;
		}
	}
	return (result);
}

/**
 * @brief Expands and splits command arguments from tokens.
 * 
 * This function processes an array of tokens, expanding environment variables
 * and splitting arguments based on whitespace. It handles quoted tokens and
 * export assignments correctly.
 * @param tokens The array of tokens to process.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A dynamically allocated array of strings representing the expanded
 *         and split arguments, or NULL on failure.
 */
// char **expand_and_split_args(t_token **tokens, t_env_var *env_list, int last_exit_status)
// {
// 	char	**final_args = NULL;
// 	int		final_count = 0;
// 	int		i = 0;

// 	while (tokens[i] != NULL)
// 	{
// 		char *expanded = expand_variables(tokens[i]->value, env_list, last_exit_status, tokens[i]->quote);
// 		char **split = NULL;
// 		if (is_export_assignment(tokens[i]) && tokens[i]->quote != NO_QUOTE)
// 		{
// 			split = malloc(sizeof(char *) * 2);
// 			if (split == NULL)
// 			{
// 				free(expanded);
// 				return (NULL);
// 			}
// 			split[0] = ft_strdup(tokens[i]->value);  // Use raw value, not expanded
// 			split[1] = NULL;
// 			free(expanded);
// 		}
// 		else if (tokens[i]->quote == SINGLE_QUOTE || tokens[i]->quote == DOUBLE_QUOTE)
// 		{
// 			split = malloc(sizeof(char *) * 2);
// 			if (split == NULL)
// 			{
// 				free(expanded);
// 				return NULL;
// 			}
// 			split[0] = expanded;
// 			split[1] = NULL;
// 		}
// 		else
// 		{
// 			split = ft_split_whitespace(expanded);
// 			free(expanded);
// 		}
// 		int split_count = 0; // Count the number of strings in split
// 		while (split && split[split_count] != NULL)
// 			split_count++;
// 		// Allocate new array with extra space
// 		char **new_final = malloc(sizeof(char *) * (final_count + split_count + 1));
// 		if (new_final == NULL)
// 			return NULL;
// 		int j = 0;
// 		while (j < final_count)
// 		{
// 			new_final[j] = final_args[j];
// 			j++;
// 		}
// 		j = 0;
// 		while (j < split_count)
// 		{
// 			new_final[final_count + j] = split[j];
// 			j++;
// 		}
// 		new_final[final_count + split_count] = NULL;
// 		free(final_args);
// 		free(split); // only pointer, not content
// 		final_args = new_final;
// 		final_count += split_count;
// 		i++;
// 	}
// 	return final_args;
// }


/**
 * @brief Handles quoted or export assignment tokens.
 * 
 * This function processes a token that is either an export assignment
 * or a quoted string. It returns a dynamically allocated array containing
 * the token's value, either raw or expanded, depending on the quote type.
 * @param token The token to process.
 * @param expanded The expanded value of the token.
 * @return A dynamically allocated array with the processed value, or NULL on failure.
 */
static char **handle_quoted_or_export_token(t_token *token, char *expanded)
{
    char **split = malloc(sizeof(char *) * 2);
    if (split == NULL)
	{
		free(expanded);
		return NULL;
	}
    if (is_export_assignment(token) && token->quote != NO_QUOTE)
    {
        split[0] = ft_strdup(token->value);  // raw unexpanded value
        free(expanded);
    }
    else
    {
        split[0] = expanded;  // expanded string, keep ownership
    }
    split[1] = NULL;
    return split;
}

/**
 * @brief Appends a split array to the final arguments array.
 * 
 * This function appends a newly created split array to the final arguments
 * array, reallocating memory as necessary. It updates the count of final
 * arguments and returns the new array.
 * @param final_args The current final arguments array.
 * @param final_count Pointer to the current count of final arguments.
 * @param split The split array to append.
 * @return A new dynamically allocated array with the split appended, or NULL on failure.
 */
static char **append_split_to_final(char **final_args, int *final_count, char **split)
{
    int split_count = 0;
    while (split && split[split_count] != NULL)
        split_count++;
    char **new_final = malloc(sizeof(char *) * (*final_count + split_count + 1));
    if (!new_final)
        return NULL;
    int i = 0;
    while (i < *final_count)
    {
        new_final[i] = final_args[i];
        i++;
    }
    int j = 0;
    while (j < split_count)
    {
        new_final[*final_count + j] = split[j];
        j++;
    }
    new_final[*final_count + split_count] = NULL;
    free(final_args);
    free(split); // free pointer only, not contents
    *final_count += split_count;
    return new_final;
}

/**
 * @brief Expands and splits command arguments from tokens.
 * 
 * This function processes an array of tokens, expanding environment variables
 * and splitting arguments based on whitespace. It handles quoted tokens and
 * export assignments correctly.
 * @param tokens The array of tokens to process.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A dynamically allocated array of strings representing the expanded
 *         and split arguments, or NULL on failure.
 */
char **expand_and_split_args(t_token **tokens, t_env_var *env_list, int last_exit_status)
{
    char **final_args = NULL;
    int final_count = 0;
    int i = 0;

    while (tokens[i] != NULL)
    {
        char *expanded = expand_variables(tokens[i]->value, env_list, last_exit_status, tokens[i]->quote);
        char **split = NULL;
        if (is_export_assignment(tokens[i]) && tokens[i]->quote != NO_QUOTE)
            split = handle_quoted_or_export_token(tokens[i], expanded);
        else if (tokens[i]->quote == SINGLE_QUOTE || tokens[i]->quote == DOUBLE_QUOTE)
            split = handle_quoted_or_export_token(tokens[i], expanded);
        else
        {
            split = ft_split_whitespace(expanded);
            free(expanded);
        }
        if (split == NULL)
            return NULL;
        char **new_final = append_split_to_final(final_args, &final_count, split);
        if (new_final == NULL)
            return NULL;
        final_args = new_final;
        i++;
    }
    return final_args;
}
