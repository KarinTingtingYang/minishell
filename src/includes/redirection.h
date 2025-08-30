/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:11:42 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 14:23:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# define MAX_HEREDOCS 16

int		is_redirection(const char *token);
void	open_and_redirect_input(char *input_file);
int		process_output_file(char *output_file, int is_append);
void	open_and_redirect_output(char *output_file, int output_mode);
void	redirect_io(char *input_file, char *output_file, int output_mode);
char	*handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status);
char	**handle_redirection(char **args, t_process_data *p_data, char **final_input_file,
			char **final_output_file, int *output_mode, char **heredoc_file);

#endif