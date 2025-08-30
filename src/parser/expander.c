/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 15:32:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

/**
 * @brief Processes the next segment of the input string for variable expansion.
 * 
 * This function checks if the current character is a '$' indicating a variable
 * expansion. If so, it delegates to the appropriate handler based on the next
 * character. If not, it appends the current character to the result.
 * 
 * @param input The input string being processed.
 * @param i The current index in the input string.
 * @param result Pointer to the result string being built.
 * @param data Pointer to the expansion processing data containing environment
 *             variables and quote context.
 * @return The updated index in the input string after processing.
 */
static size_t	process_next_segment(const char *input, size_t i, char **result,
										t_exp_proc_data *data)
{
	char	next_char;

	if (input[i] == '$' && input[i + 1] && data->quote_char != '\'')
	{
		next_char = input[i + 1];
		if (next_char == '\'' || next_char == '"')
			return (i + 1);
		if (ft_isalnum(next_char) || next_char == '_'
			|| next_char == '?' || next_char == '{')
		{
			return (handle_variable_expansion(input, i, result,
					data->exp_data));
		}
		else
		{
			*result = append_char(*result, input[i]);
			return (i + 1);
		}
	}
	else
	{
		*result = append_char(*result, input[i]);
		return (i + 1);
	}
}

/**
 * @brief Expands variables in the input string based on the environment list
 * and last exit status.
 * 
 * This function processes the input string character by character, expanding
 * variables as needed while respecting quote contexts. It returns a newly
 * allocated string with all expansions applied.
 * 
 * @param input The input string containing potential variables to expand.
 * @param env_list The linked list of environment variables for expansion.
 * @param last_exit_status The last exit status for special variable expansion.
 * @param quote The quote type context (not used in current implementation).
 * @return A newly allocated string with expanded variables, or NULL on failure.
 */
char	*expand_variables(char *input, t_env_var *env_list,
							int last_exit_status, t_quote_type quote)
{
	char			*result;
	size_t			i;
	t_expand_data	expand_data;
	t_exp_proc_data	proc_data;

	(void)quote;
	expand_data.env_list = env_list;
	expand_data.last_exit_status = last_exit_status;
	proc_data.exp_data = &expand_data;
	result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	i = 0;
	proc_data.quote_char = 0;
	while (input[i] != '\0')
	{
		if (proc_data.quote_char == 0 && (input[i] == '\'' || input[i] == '"'))
			proc_data.quote_char = input[i];
		else if (input[i] == proc_data.quote_char)
			proc_data.quote_char = 0;
		i = process_next_segment(input, i, &result, &proc_data);
	}
	return (result);
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
static char	**process_token_for_expansion(t_token *token,
		const char *prev_token_val, t_env_var *env_list, int last_exit_status)
{
	char	*expanded;
	char	**split;

	if (prev_token_val && ft_strncmp(prev_token_val, "<<", 3) == 0)
	{
		split = malloc(sizeof(char *) * 2);
		if (split == NULL)
			return (NULL);
		split[0] = ft_strdup(token->value);
		split[1] = NULL;
	}
	else
	{
		expanded = expand_variables(token->value, env_list,
				last_exit_status, token->quote);
		if (expanded == NULL)
			return (NULL);
		split = process_token(token, expanded);
		if (split == NULL)
			return (NULL);
	}
	return (split);
}

/**
 * @brief Expands variables in a list of tokens and splits them into arguments.
 * 
 * This function processes each token, expands any variables, and handles
 * whitespace splitting based on the original quotes. It returns a newly
 * allocated array of strings representing the final arguments.
 * 
 * @param tokens The array of tokens to process.
 * @param env_list The linked list of environment variables for expansion.
 * @param last_exit_status The last exit status for special variable expansion.
 * @return A pointer to the newly allocated array of argument strings,
 *         or NULL on failure.
 */
char	**expand_and_split_args(t_token **tokens,
									t_env_var *env_list, int last_exit_status)
{
	char		**final_args;
	int			final_count;
	char		**split;
	int			i;
	const char	*prev_val;

	final_args = NULL;
	final_count = 0;
	i = 0;
	while (tokens[i] != NULL)
	{
		if (i > 0)
			prev_val = tokens[i - 1]->value;
		else
			prev_val = NULL;
		split = process_token_for_expansion(tokens[i], prev_val,
				env_list, last_exit_status);
		if (split == NULL)
			return (NULL);
		final_args = append_split_to_final(final_args, &final_count, split);
		if (final_args == NULL)
			return (NULL);
		i++;
	}
	return (final_args);
}
