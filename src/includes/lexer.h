/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:04 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 15:04:43 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

char	**tokenize_input(char *line);
void	free_tokens(t_token **tokens);
char	**print_error(const char *msg);
char	**split_line_by_pipe(char *line);
void	append_to_token(char c, char **buffer);
char	**save_token(char **tokens, char *token, int *count);

#endif