/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 12:13:56 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/08 12:46:09 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
 * @brief Processes a single token and appends its expanded
 *        arguments to a final list.
 *
 * This function handles the logic for expanding variables within a single token,
 * splitting the result, and appending it to the main arguments array. It also
 * checks for and handles syntax errors.
 *
 * @param token The token to process.
 * @param pdata The process data containing syntax error flags.
 * @param final_args The current list of final arguments.
 * @param final_count A pointer to the count of arguments in final_args.
 * @return The updated array of arguments, or NULL on failure.
 */
// static char	**append_expanded_token(t_token *token, t_process_data *pdata,
// 											char **final_args, int *final_count)
// {
// 	char		**split;
// 	const char	*prev_val;

// 	assign_prev_val_value((char **)&prev_val, &token, 0);
// 	split = process_token_for_expansion(token, prev_val, pdata);
// 	if (split == NULL)
// 		return (free_split(final_args), NULL);
// 	if (pdata->syntax_error)
// 	{
// 		free_split(split);
// 		free_split(final_args);
// 		return (NULL);
// 	}
// 	return (append_split_to_final(final_args, final_count, split));
// }
static int	append_expanded_token(t_token **tokens, int i,
									t_process_data *pdata,
									t_append_info *arg_info)
{
	char		**split;
	const char	*prev_val;
	char		**new_args;

	assign_prev_val_value((char **)&prev_val, tokens, i);
	split = process_token_for_expansion(tokens[i], prev_val, pdata);
	if (split == NULL)
	{
		free_split(arg_info->array);
		arg_info->array = NULL;
		return (-1);
	}
	if (pdata->syntax_error)
	{
		free_split(split);
		free_split(arg_info->array);
		arg_info->array = NULL;
		return (-1);
	}
	new_args = append_split_to_final(arg_info->array, &arg_info->count, split);
	if (new_args == NULL)
		return (-1);
	arg_info->array = new_args;
	return (0);
}

/**
 * @brief Expands variables and splits tokens into final arguments.
 *
 * This function iterates through a list of tokens, using a helper function
 * to expand variables, handle quotes, and build the final array of arguments.
 * It is now much shorter and easier to read.
 *
 * @param tokens The array of tokens to process.
 * @param pdata The process data structure.
 * @return A pointer to the newly allocated array of argument strings, or NULL
 * on failure.
 */
// char	**expand_and_split_args(t_token **tokens, t_process_data *pdata)
// {
// 	char	**final_args;
// 	int		final_count;
// 	int		i;
// 	char	**tmp;

// 	final_args = NULL;
// 	final_count = 0;
// 	i = 0;
// 	pdata->syntax_error = 0;
// 	while (tokens[i] != NULL)
// 	{
// 		tmp = append_expanded_token(tokens[i], pdata, final_args, &final_count);
// 		if (tmp == NULL)
// 			return (NULL);
// 		final_args = tmp;
// 		i++;
// 	}
// 	return (final_args);
// }
char	**expand_and_split_args(t_token **tokens, t_process_data *pdata)
{
	t_append_info	arg_info;
	int				i;

	arg_info.array = NULL;
	arg_info.count = 0;
	i = 0;
	pdata->syntax_error = 0;
	while (tokens[i] != NULL)
	{
		if (append_expanded_token(tokens, i, pdata, &arg_info) == -1)
			return (NULL);
		i++;
	}
	return (arg_info.array);
}
