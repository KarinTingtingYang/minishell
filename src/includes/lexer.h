/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 08:42:04 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 11:39:08 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

/** @brief Struct to hold information for appending strings.
 *
 * This struct contains an array of strings and a count of how many
 * strings are currently in the array. It is used to manage dynamic
 * appending of strings.
 */
typedef struct s_append_info
{
	char	**array;
	int		count;
}	t_append_info;

void	free_tokens(t_token **tokens);
char	*append_char(char *str, char c);
char	**split_line_by_pipe(char *line);
char	**tokenize_input(char *line, t_process_data *pd);
char	**duplicate_tokens_to_array(t_token **tokens, int len);
char	**append_str(t_append_info *info, char *line, int start, int end);

#endif