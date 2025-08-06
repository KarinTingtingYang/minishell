/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/26 10:54:01 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/06 09:13:30 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include <stdio.h>

typedef struct s_command t_command;

typedef enum e_quote_type {
    NO_QUOTE,
    SINGLE_QUOTE,
    DOUBLE_QUOTE
} t_quote_type;

typedef struct s_token {
    char *value;
    t_quote_type quote;
} t_token;

t_token **parse_line(char *line);
void		free_command(t_command *cmd);
t_command	*create_command(char **tokens, char **path_dirs);
char		**save_token(char **tokens, char *token, int *count);
char		**print_error(const char *msg);
char		*append_char(char *str, char c);
char		**append_str(char **arr, char *line, int start, int end, int *count);
char		**tokenize_input(char *line);
char		**split_line_by_pipe(char *line);

#endif
