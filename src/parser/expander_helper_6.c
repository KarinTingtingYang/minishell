/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_helper_6.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/04 13:16:52 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/04 14:10:39 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Creates a split array for heredoc tokens.
 * 
 * This function creates a new array containing the token's value
 * as the only element, followed by a NULL terminator. This is used
 * specifically for heredoc delimiters which should not be split.
 * 
 * @param token The token representing the heredoc delimiter.
 * @return A newly allocated array with the token's value and NULL,
 *         or NULL on allocation failure.
 */
static char	**make_heredoc_split(t_token *token)
{
	char	**split;

	split = malloc(sizeof(char *) * 2);
	if (split == NULL)
		return (NULL);
	split[0] = ft_strdup(token->value);
	if (split[0] == NULL)
	{
		free(split);
		return (NULL);
	}
	split[1] = NULL;
	return (split);
}

/**
 * @brief Processes a token after variable expansion.
 * 
 * This function handles the token based on its quote type and whether
 * it is an export assignment. It may split the expanded string based
 * on whitespace or remove quotes as needed.
 * 
 * @param token The original token before expansion.
 * @param expanded The expanded string after variable expansion.
 * @return A newly allocated array of strings after processing,
 *         or NULL on failure.
 */
char	**process_token_for_expansion(t_token *token,
		const char *prev_token_val, t_process_data *pdata)
{
	char	*expanded;
	char	**split;

	if (prev_token_val && ft_strncmp(prev_token_val, "<<", 3) == 0)
		return (make_heredoc_split(token));
	else
	{
		expanded = expand_variables(token->value, pdata->env_list,
				pdata->last_exit_status, token->quote);
		if (expanded == NULL)
			return (NULL);
		if (prev_token_val && is_redirection((char *)prev_token_val)
			&& ft_strncmp(prev_token_val, "<<", 3) != 0)
		{
			if (is_ambiguous_redirect(expanded, token->value, pdata))
				return (free(expanded), expanded = NULL, NULL);
		}
		split = process_token(token, expanded);
		if (split == NULL)
			return (NULL);
	}
	return (split);
}
