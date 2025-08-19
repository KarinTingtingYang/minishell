/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 12:13:56 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/19 11:33:45 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**append_split_to_final(char **final_args,
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

// static char	**process_token(t_token *token, char *expanded)
// {
// 	char	**split;

// 	if (is_export_assignment(token) || token->quote != NO_QUOTE
// 		//|| ft_strchr(expanded, '\'') || ft_strchr(expanded, '\"'))
// 		|| ft_strchr(token->value, '\'') || ft_strchr(token->value, '\"')) // DEBUG
// 	{
// 		// debug
// 		printf("process_token triggered condition #1\n");
// 		split = handle_quoted_or_export_token(token, expanded);
// 	}
// 	else
// 	{
// 		printf("process_token triggered condition #2\n"); // debug
// 		split = handle_whitespace_splitting(expanded);
// 	}
		
// 	return (split);
// }

static int had_original_quotes(const t_token *token)
{
    const char *s;

    if (!token || !token->value)
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

static char **process_token(t_token *token, char *expanded)
{
    char **split;

    if (is_export_assignment(token))
        return handle_quoted_or_export_token(token, expanded);

    if (token->quote != NO_QUOTE)
    {
        expanded = remove_outer_quotes(expanded);
        if (!expanded)
            return (NULL);
        return handle_quoted_or_export_token(token, expanded);
    }
    else
    {
        if (had_original_quotes(token))
        {
            char *joined = remove_quotes_and_join(expanded);
            if (!joined)
            {
                free(expanded);
                return (NULL);
            }
            free(expanded);
            split = handle_whitespace_splitting(joined);
        }
        else
        {
            split = handle_whitespace_splitting(expanded);
        }
    }
    return (split);
}


size_t	handle_variable_expansion(const char *input, size_t i,
											char **result, t_expand_data *data)
{
	if (input[i + 1] == '{')
		return (expand_braced_variable(input, i, result, data));
	else
		return (expand_simple_variable(input, i, result, data));
}

char	*expand_variables(char *input, t_env_var *env_list,
								int last_exit_status, t_quote_type quote)
{
	char			*result;
	size_t			i;
	t_expand_data	expand_data;
	char			quote_char;

	// if (quote == SINGLE_QUOTE)
	// 	return (ft_strdup(input));
	(void)quote;
	expand_data.env_list = env_list;
	expand_data.last_exit_status = last_exit_status;
	result = ft_strdup("");
	if (result == NULL)
		return (NULL);
	i = 0;
	quote_char = 0;
	while (input[i] != '\0')
	{
		// Update quote state
		if (quote_char == 0 && (input[i] == '\'' || input[i] == '"'))
			quote_char = input[i];
		else if (input[i] == quote_char)
			quote_char = 0;
		
		//if (input[i] == '$' && input[i + 1])
		if (input[i] == '$' && input[i + 1] && quote_char != '\'')
			//i = handle_variable_expansion(input, i, &result, &expand_data);
		{
			// A '$' followed by something that isn't a valid variable
			// (e.g., a quote, space, or null) is treated as a literal '$'.
			char next_char = input[i + 1];
			// FIX: If '$' is followed by a quote, skip the '$' entirely.
			if (next_char == '\'' || next_char == '"')
			{
				i++; // Skips the '$'
				continue; // The loop will then process the quote
			}
			if (ft_isalnum(next_char) || next_char == '_' || next_char == '?')
				i = handle_variable_expansion(input, i, &result, &expand_data);
			else
			{
				result = append_char(result, input[i]);
				i++;
			}
		}
		else
		{
			result = append_char(result, input[i]);
			i++;
		}
	}
	return (result);
}

char	**expand_and_split_args(t_token **tokens,
									t_env_var *env_list, int last_exit_status)
{
	char	**final_args;
	int		final_count;
	char	*expanded;
	char	**split;
	int		i;

	final_args = NULL;
	final_count = 0;
	i = 0;
	while (tokens[i] != NULL)
	{
		// FIX: Prevent expansion of the heredoc delimiter
		if (i > 0 && ft_strncmp(tokens[i - 1]->value, "<<", 3) == 0)
		{
			// For the delimiter, we do NOT expand variables and we do NOT remove quotes here.
			// We just create a simple "split" array containing the raw delimiter.
			split = malloc(sizeof(char *) * 2);
			if (!split)
				return (NULL);
			split[0] = ft_strdup(tokens[i]->value);
			split[1] = NULL;
		}
		else
		{
			expanded = expand_variables(tokens[i]->value, env_list,
				last_exit_status, tokens[i]->quote);
			printf("expand_variables returns [%s]\n", expanded); // DEBUG
			if (expanded == NULL)
				return (NULL);
			split = process_token(tokens[i], expanded);
			if (split == NULL)
				return (NULL);
			printf("process_token returns:\n");
			print_array(split);
		}
		final_args = append_split_to_final(final_args, &final_count, split);
		if (final_args == NULL)
			return (NULL);
		i++;
	}
	return (final_args);
}
