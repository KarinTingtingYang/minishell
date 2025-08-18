/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:14:24 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/18 12:25:08 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

typedef struct s_expand_data
{
	t_env_var	*env_list;
	int			last_exit_status;
}	t_expand_data;

char	**ft_split_whitespace(char *s);
char	*remove_quotes_and_join(char *str);
int		is_export_assignment(t_token *token);
void	copy_original_arguments(char **new_final,
			char **final_args, int final_count);
char	**expand_and_split_args(t_token **tokens,
			t_env_var *env_list, int last_exit_status);
char	*get_var_value(const char *var_name,
			t_env_var *env_list, int last_exit_status);
char	**handle_whitespace_splitting(char *expanded);
void	copy_new_arguments(char **new_final, char **split,
			int final_count, int split_count);
int		expand_simple_variable(const char *input, size_t i,
			char **result, t_expand_data *data);
int		expand_braced_variable(const char *input, size_t i,
			char **result, t_expand_data *data);
int		handle_standard_var(const char *input, size_t start,
			char **result, t_expand_data *data);
char	**handle_quoted_or_export_token(t_token *token, char *expanded);
int		handle_default_case(const char *input, size_t i, char **result);
char	*expand_variables(char *input, t_env_var *env_list,
								int last_exit_status, t_quote_type quote);
char	*remove_outer_quotes(char *str);

#endif
