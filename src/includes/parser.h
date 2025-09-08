/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:54:01 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 11:39:16 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>

typedef struct s_command		t_command;

typedef struct s_execute_data	t_execute_data;

/** @brief Enum to represent the type of quotes in a token.
 *
 * This enum defines the possible quote types for a token:
 * - NO_QUOTE: The token is not quoted.
 * - SINGLE_QUOTE: The token is enclosed in single quotes.
 * - DOUBLE_QUOTE: The token is enclosed in double quotes.
 */
typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

/** @brief Struct to represent a token with its value and quote type.
 *
 * This struct holds the value of the token as a string and the
 * type of quotes that enclose it, if any.
 */
typedef struct s_token
{
	char			*value;
	t_quote_type	quote;
}	t_token;

int			is_redirect(const char *s);
int			count_tokens(const char *s);
void		free_command(t_command *cmd);
int			count_heredocs(const char *line);
char		**append_split_to_final(char **final_args,
				int *final_count, char **split);
void		free_partial_strings(char **arr, int upto);
int			validate_redirect_syntax(t_token **tokens);
char		*substr_dup(const char *start, size_t len);
t_token		**parse_line(char *line, t_process_data *pd);
char		**process_token(t_token *token, char *expanded);
t_command	*create_command(char **tokens, char **path_dirs,
				t_process_data *process_data);
int			cleanup_and_return_error(t_execute_data exec_data);
t_token		*create_token(char *value, t_quote_type quote_type);
int			quotes_are_closed(const char *line, t_process_data *pd);
int			precheck_redir_syntax(const char *line, t_process_data *pd);
int			is_valid_token_after_redir(const char *line, int i, int op_len,
				t_process_data *pd);

#endif
