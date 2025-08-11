/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 10:44:35 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 15:04:57 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/** @brief Prints an error message to stderr.
 * 
 * This function prints a formatted error message to the standard error output.
 * It prefixes the message with "minishell: " for clarity.
 * @param msg The error message to print.
 * @return A NULL pointer to indicate an error occurred.
 */
char **print_error(const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (NULL);
}

/** @brief Appends a character to a dynamically allocated string.
 * 
 * This function appends a single character to the end of a string,
 * reallocating memory as necessary. If the input string is NULL, it
 * initializes it with the character.
 * @param str The original string, or NULL if empty.
 * @param c The character to append.
 * @return A new string with the character appended, or NULL on failure.
 */
char *append_char(char *str, char c)
{
	int		len;
	char	*new_str;

	if (str != NULL)
		len = ft_strlen(str);
	else
		len = 0;
	new_str = malloc(len + 2);
	if (new_str == NULL)
		return (NULL);
	if (str != NULL)
	{
		ft_strlcpy(new_str, str, len + 1);
		free(str);
	}
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

/** @brief Saves a token into an array of tokens.
 * 
 * This function appends a new token to an existing array of tokens,
 * reallocating memory as necessary. It also updates the count of tokens.
 * @param tokens The current array of tokens, or NULL if empty.
 * @param token The token to save.
 * @param count Pointer to the current count of tokens, which will be updated.
 * @return A new array of tokens with the new token added, or NULL on failure.
 */
// REDIRECTION DEBUGGING: we can reused the parse_line function
// we don't need this function anymore

// char **save_token(char **tokens, char *token, int *count)
// {
// 	char	**new_tokens;
// 	int		i = 0;

// 	new_tokens = malloc(sizeof(char *) * (*count + 2));
// 	if (new_tokens == NULL)
// 		return (NULL);
// 	while (i < *count)
// 	{
// 		new_tokens[i] = tokens[i];
// 		i++;
// 	}
// 	new_tokens[i++] = token;
// 	new_tokens[i] = NULL;
// 	free(tokens);
// 	(*count)++;
// 	return (new_tokens);
// }

/** @brief Appends a substring to an array of strings.
 * 
 * This function appends a substring from the input line to an array of strings,
 * reallocating memory as necessary. It updates the count of strings in the array.
 * @param arr The current array of strings, or NULL if empty.
 * @param line The input line containing the substring.
 * @param start The starting index of the substring in the line.
 * @param end The ending index (exclusive) of the substring in the line.
 * @param count Pointer to the current count of strings, which will be updated.
 * @return A new array of strings with the substring added, or NULL on failure.
 */
char **append_str(char **arr, char *line, int start, int end, int *count)
{
    int len = end - start;
    char *substr = malloc(len + 1);
	char **new_arr ;
	
    if (substr == NULL)
        return NULL;
    ft_strlcpy(substr, line + start, len + 1);
    substr[len] = '\0';
    new_arr = realloc(arr, sizeof(char *) * (*count + 2));
    if (new_arr == NULL)
	{
		free(substr);
		return NULL;
	}
    new_arr[*count] = substr;
    (*count)++;
    new_arr[*count] = NULL;
    return (new_arr);
}

/** @brief Frees an array of t_token structures.
 * 
 * This function iterates through an array of t_token pointers, freeing
 * each token's value and the token itself. It then frees the array of tokens.
 * @param tokens The array of t_token pointers to free.
 */
void free_tokens(t_token **tokens)
{
    int i = 0;

    if (tokens == NULL)
        return ;
    while (tokens[i] != NULL)
	{
		free(tokens[i]->value);
		free(tokens[i]);
		i++;
	}
    free(tokens);
}

