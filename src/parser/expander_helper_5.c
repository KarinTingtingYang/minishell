/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:19:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 15:36:13 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Appends newly split arguments to the final arguments array.
 * 
 * This function creates a new array that combines the original final
 * arguments with the newly split arguments. It frees the old final
 * arguments and split arrays.
 * @param final_args The original final arguments array.
 * @param final_count Pointer to the count of final arguments, which will
 *                    be updated.
 * @param split The newly split arguments array.
 * @return A pointer to the new combined array, or NULL on failure.
 */
char	**append_split_to_final(char **final_args,
										int *final_count, char **split)
{
	int		split_count;
	char	**new_final;

	split_count = 0;
	while (split && split[split_count] != NULL)
		split_count++;
	new_final = malloc(sizeof(char *) * (*final_count + split_count + 1));
	if (new_final == NULL)
		return (NULL);
	copy_original_arguments(new_final, final_args, *final_count);
	copy_new_arguments(new_final, split, *final_count, split_count);
	new_final[*final_count + split_count] = NULL;
	free(final_args);
	free(split);
	*final_count += split_count;
	return (new_final);
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
