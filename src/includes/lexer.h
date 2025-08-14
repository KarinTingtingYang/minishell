/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:04 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/14 11:09:24 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef struct s_append_info
{
	char	**array;
	int		count;
}	t_append_info;

char	**tokenize_input(char *line);
void	free_tokens(t_token **tokens);
char	*append_char(char *str, char c);
char	**split_line_by_pipe(char *line);
char	**duplicate_tokens_to_array(t_token **tokens, int len);
char	**append_str(t_append_info *info, char *line, int start, int end);

#endif