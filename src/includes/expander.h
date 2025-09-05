/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:14:24 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/05 14:01:53 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

/** @brief Struct to hold data needed for variable expansion.
 *
 * This struct contains the environment variable list and the last
 * exit status, which are used during variable expansion.
 */
typedef struct s_expand_data
{
	t_env_var	*env_list;
	int			last_exit_status;
}	t_expand_data;

/** @brief Struct to hold state information during expansion processing.
 *
 * This struct is used to pass multiple state variables to the recursive
 * function as a single argument, reducing the argument count.
 */
typedef struct s_exp_proc_data
{
	t_expand_data	*exp_data;
	char			quote_char;
}	t_exp_proc_data;

char	**ft_split_whitespace(char *s);
char	*remove_outer_quotes(char *str);
char	*remove_quotes_and_join(char *str);
int		is_export_assignment(t_token *token);
char	**handle_whitespace_splitting(char *expanded);
char	**expand_and_split_args(t_token **tokens,
			t_process_data *process_data);
char	*get_var_value(const char *var_name,
			t_env_var *env_list, int last_exit_status);
char	*expand_variables(char *input, t_env_var *env_list,
			int last_exit_status, t_quote_type quote);
int		expand_simple_variable(const char *input, size_t i,
			char **result, t_expand_data *data);
int		expand_braced_variable(const char *input, size_t i,
			char **result, t_expand_data *data);
int		handle_standard_var(const char *input, size_t start,
			char **result, t_expand_data *data);
size_t	handle_variable_expansion(const char *input, size_t i,
			char **result, t_expand_data *data);
char	**handle_quoted_or_export_token(t_token *token, char *expanded);
int		handle_default_case(const char *input, size_t i, char **result);
int		is_ambiguous_redirect(const char *expanded_value,
			const char *original_value, t_process_data *pdata);
char	**process_token_for_expansion(t_token *token,
			const char *prev_token_val, t_process_data *pdata);

#endif
