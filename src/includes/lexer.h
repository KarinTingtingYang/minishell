/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:04 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 11:44:42 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef LEXER_H
#define LEXER_H

typedef enum e_quote_state {
    OUTSIDE_QUOTE,
    IN_SINGLE_QUOTE,
    IN_DOUBLE_QUOTE
} t_quote_state;

char **tokenize_input(char *line);
void append_to_token(char c, char **buffer);
char **save_token(char **tokens, char *token, int *count);
char **print_error(const char *msg);
char **split_line_by_pipe(char *line);

#endif