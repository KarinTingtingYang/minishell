/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:47:19 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Splits a string into an array of words based on whitespace.
 * 
 * This function uses ft_split_whitespace to split the expanded string
 * into an array of words. It frees the original expanded string.
 * @param expanded The expanded string to split.
 * @return A pointer to the newly allocated array of strings,
 *         or NULL on failure.
 */
char	**handle_whitespace_splitting(char *expanded)
{
	char	**split;

	split = ft_split_whitespace(expanded);
	free(expanded);
	return (split);
}

/**
 * @brief Checks if a token represents an export assignment.
 * 
 * This function checks if the token's value contains an '=' character
 * and if the part before '=' is a valid identifier.
 * @param token The token to check.
 * @return 1 if the token is an export assignment, 0 otherwise.
 */
int	is_export_assignment(t_token *token)
{
	char	*equal_sign_pos;
	char	*key;
	int		is_valid;

	equal_sign_pos = ft_strchr(token->value, '=');
	if (equal_sign_pos == NULL)
		return (0);
	if (equal_sign_pos == token->value)
		return (0);
	key = ft_strndup(token->value, equal_sign_pos - token->value);
	if (!key)
		return (0);
	is_valid = is_valid_identifier(key);
	free(key);
	return (is_valid);
}

/**
 * @brief Handles tokens that are either quoted or export assignments.
 * 
 * This function processes the token based on its type. If it's an export
 * assignment or quoted, it keeps the expanded string as is. Otherwise,
 * it removes quotes and joins the string.
 * @param token The token being processed.
 * @param expanded The expanded string of the token.
 * @return A pointer to the newly allocated array of strings,
 *         or NULL on failure.
 */
char	**handle_quoted_or_export_token(t_token *token, char *expanded)
{
	char	**split;
	char	*joined;

	split = malloc(sizeof(char *) * 2);
	if (split == NULL)
	{
		free(expanded);
		return (NULL);
	}
	if (is_export_assignment(token))
		split[0] = expanded;
	else if (token->quote != NO_QUOTE)
		split[0] = expanded;
	else
	{
		joined = remove_quotes_and_join(expanded);
		free(expanded);
		if (joined == NULL)
			return (free(split), NULL);
		split[0] = joined;
	}
	split[1] = NULL;
	return (split);
}

/**
 * @brief Handles the default case when expanding variables.
 * 
 * This function appends the current character to the result string
 * when no variable expansion is needed.
 * @param input The original input string.
 * @param i The current index in the input string.
 * @param result Pointer to the result string being built.
 * @return The next index in the input string.
 */
int	handle_default_case(const char *input, size_t i, char **result)
{
	*result = append_char(*result, input[i]);
	return (i + 1);
}

/**
 * @brief Expands a standard variable (e.g., $VAR) in the input string.
 * 
 * This function extracts the variable name, retrieves its value, and
 * appends it to the result string.
 * @param input The original input string.
 * @param start The starting index of the variable name in the input.
 * @param result Pointer to the result string being built.
 * @param data The expansion data containing environment variables and
 * last exit status.
 * @return The index in the input string after processing the variable.
 */
int	handle_standard_var(const char *input, size_t start,
							char **result, t_expand_data *data)
{
	size_t	var_start;
	char	*var_name;
	char	*value;
	char	*tmp;

	var_start = start;
	while (input[start] && (ft_isalnum(input[start]) || input[start] == '_'))
		start++;
	var_name = ft_strndup(&input[var_start], start - var_start);
	value = get_var_value(var_name, data->env_list, data->last_exit_status);
	tmp = ft_strjoin(*result, value);
	free(*result);
	*result = tmp;
	free(var_name);
	free(value);
	return (start);
}
