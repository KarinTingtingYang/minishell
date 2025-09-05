/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:13:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/04 16:38:58 by makhudon         ###   ########.fr       */
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
 * @brief Assigns the previous token's value for context during expansion.
 * 
 * This helper function sets the `prev_val` pointer to the value of the
 * previous token in the array, or NULL if processing the first token.
 * 
 * @param prev_val Pointer to store the previous token's value.
 * @param tokens The array of tokens being processed.
 * @param i The current index in the tokens array.
 */
static void	assign_prev_val_value(char **prev_val, t_token **tokens, int i)
{
	if (i > 0)
		*prev_val = tokens[i - 1]->value;
	else
		*prev_val = NULL;
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
char	**expand_and_split_args(t_token **tokens, t_process_data *pdata)
{
	char		**final_args;
	int			final_count;
	char		**split;
	int			i;
	const char	*prev_val;

	final_args = NULL;
	final_count = 0;
	i = 0;
	pdata->syntax_error = 0;
	while (tokens[i] != NULL)
	{
		assign_prev_val_value((char **)&prev_val, tokens, i);
		split = process_token_for_expansion(tokens[i], prev_val, pdata);
		if (split == NULL)
			return (free_split(final_args), NULL);
		if (pdata->syntax_error)
			return (free_split(split), free_split(final_args), NULL);
		final_args = append_split_to_final(final_args, &final_count, split);
		if (final_args == NULL)
			return (NULL);
		i++;
	}
	return (final_args);
}
