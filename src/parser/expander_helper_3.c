/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_helper_3.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/05 10:39:19 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/19 10:49:36 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**handle_whitespace_splitting(char *expanded)
{
	char	**split;

	split = ft_split_whitespace(expanded);
	free(expanded);
	return (split);
}

/**
 * @brief Expands environment variables in a token.
 * 
 * This function processes a token to expand any environment variables
 * it contains. It handles both single and double quotes, ensuring that
 * variables are only expanded outside of quotes.
 * @param token The token to process.
 * @param env_list The linked list of environment variables.
 * @param last_exit_status The last exit status of the shell.
 * @return A new token with expanded variables, or NULL on failure.
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
 * @brief Handles quoted or export assignment tokens.
 * 
 * This function processes a token that is either an export assignment
 * or a quoted token, returning a split array with the appropriate content.
 * @param token The token to process.
 * @param expanded The expanded string from the token.
 * @return A dynamically allocated array of strings representing
 *         the processed token, or NULL on failure.
 */
// char	**handle_quoted_or_export_token(t_token *token, char *expanded)
// {
// 	char	**split;
// 	char	*joined_str;

// 	split = malloc(sizeof(char *) * 2);
// 	if (split == NULL)
// 	{
// 		free(expanded);
// 		return (NULL);
// 	}
// 	if (is_export_assignment(token))
// 	{
// 		split[0] = ft_strdup(token->value);
// 		free(expanded);
// 	}
// 	else
// 	{
// 		joined_str = remove_quotes_and_join(expanded);
// 		free(expanded);
// 		expanded = joined_str;
// 		split[0] = expanded;
// 	}
// 	split[1] = NULL;
// 	return (split);
// }
char	**handle_quoted_or_export_token(t_token *token, char *expanded)
{
	char	**split;

	split = malloc(sizeof(char *) * 2);
	if (!split)
	{
		free(expanded);
		return (NULL);
	}
	if (is_export_assignment(token))
	{
		// split[0] = ft_strdup(token->value);
		// free(expanded);
		// if (!split[0]) { free(split); return (NULL); }
		split[0] = expanded;
	}
	else if (token->quote != NO_QUOTE)
	{
		split[0] = expanded;
	}
	else
	{
		char *joined = remove_quotes_and_join(expanded);
		free(expanded);
		if (!joined) { free(split); return (NULL); }
		split[0] = joined;
	}
	split[1] = NULL;
	return (split);
}


int	handle_default_case(const char *input, size_t i, char **result)
{
	*result = append_char(*result, input[i]);
	return (i + 1);
}

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
