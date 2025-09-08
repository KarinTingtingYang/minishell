/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_7.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 09:17:31 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 10:04:29 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees an array of strings up to a specified index.
 *
 * This function is a safety mechanism, primarily used to clean up
 * memory in a loop where an allocation failed partway through. It
 * iterates through the array from the beginning and frees each
 * string pointer until it reaches the specified index `upto`.
 * This prevents memory leaks by ensuring that all successfully
 * allocated strings are freed if a subsequent allocation fails.
 *
 * @param arr The array of strings to be partially freed.
 * @param upto The index up to which strings should be freed (exclusive).
 */
void	free_partial_strings(char **arr, int upto)
{
	int	k;

	k = 0;
	while (k < upto)
	{
		if (arr[k])
			free(arr[k]);
		k++;
	}
}

/**
 * @brief Checks if the token had original quotes.
 * 
 * This function checks if the token's value contains any quote characters.
 * @param token The token to check.
 * @return 1 if the token had quotes, 0 otherwise.
 */
static int	had_original_quotes(const t_token *token)
{
	const char	*s;

	if (token == NULL || token->value == NULL)
		return (0);
	s = token->value;
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}

/**
 * @brief Processes a token after variable expansion.
 * 
 * This function handles the token based on whether it had original quotes.
 * If it had quotes, it removes them and returns a single-element array.
 * Otherwise, it splits the expanded string on whitespace.
 * @param token The token being processed.
 * @param expanded The expanded string of the token.
 * @return A pointer to the newly allocated array of strings,
 *         or NULL on failure.
 */
char	**process_token(t_token *token, char *expanded)
{
	char	**split;
	char	*unquoted;

	if (had_original_quotes(token))
	{
		unquoted = remove_quotes_and_join(expanded);
		free(expanded);
		expanded = NULL;
		if (unquoted == NULL)
			return (NULL);
		split = (char **)malloc(sizeof(char *) * 2);
		if (split == NULL)
		{
			free(unquoted);
			return (NULL);
		}
		split[0] = unquoted;
		split[1] = NULL;
		return (split);
	}
	split = handle_whitespace_splitting(expanded);
	return (split);
}

/**
 * @brief Checks for an ambiguous redirect after variable expansion.
 *
 * This function determines if a variable expansion in a redirection
 * context results in a valid single filename. An **ambiguous redirect**
 * occurs when the expanded value is either empty or contains whitespace,
 * which would lead to an invalid filename for redirection. For example,
 * if `$VAR` is `""` or `"file with spaces"`, this function will detect an
 * ambiguous redirect.
 *
 * @param expanded_value The string after variable expansion has been applied.
 * @param original_value The original, unexpanded string (e.g., "$VAR"),
 * used for the error message.
 * @param pdata A pointer to the main process data struct, used to set the
 * shell's exit status and syntax error flag on failure.
 * @return Returns 1 if an ambiguous redirect is detected, indicating an error.
 * Returns 0 if the expanded value is a single valid token, allowing
 * redirection to proceed.
 */
int	is_ambiguous_redirect(const char *expanded_value,
				const char *original_value, t_process_data *pdata)
{
	char	**split;
	int		count;

	split = ft_split_whitespace((char *)expanded_value);
	count = 0;
	if (split)
	{
		while (split[count])
			count++;
		free_split(split);
	}
	if (count != 1)
	{
		ft_error((char *)original_value, "ambiguous redirect");
		pdata->last_exit_status = 1;
		pdata->syntax_error = 1;
		return (1);
	}
	return (0);
}

/**
 * @brief Handles variable expansion based on the character following '$'.
 * 
 * This function checks if the character after '$' is '{', indicating a
 * braced variable, and calls the appropriate expansion function. If not,
 * it treats it as a simple variable.
 * 
 * @param input The input string containing the variable.
 * @param i The current index in the input string (pointing to '$').
 * @param result Pointer to the result string being built.
 * @param data Pointer to the expansion data containing environment variables
 *             and last exit status.
 * @return The updated index in the input string after processing the variable.
 */
size_t	handle_variable_expansion(const char *input, size_t i,
											char **result, t_expand_data *data)
{
	if (input[i + 1] == '{')
		return (expand_braced_variable(input, i, result, data));
	else
		return (expand_simple_variable(input, i, result, data));
}
