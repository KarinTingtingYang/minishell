/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:54:01 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 15:25:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>

typedef struct s_command	t_command;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE
}	t_quote_type;

typedef struct s_token
{
	char			*value;
	t_quote_type	quote;
}	t_token;

t_token		**parse_line(char *line);
int			is_redirect(const char *s);
int			count_tokens(const char *s);
void		free_command(t_command *cmd);
int			count_heredocs(const char *line);
int			quotes_are_closed(const char *line);
char		**append_split_to_final(char **final_args,
				int *final_count, char **split);
int			validate_redirect_syntax(t_token **tokens);
char		*substr_dup(const char *start, size_t len);
char		**process_token(t_token *token, char *expanded);
t_command	*create_command(char **tokens, char **path_dirs,
				t_process_data *process_data);
t_token		*create_token(char *value, t_quote_type quote_type);
int			precheck_redir_syntax(const char *line, t_process_data *pd);

#endif
