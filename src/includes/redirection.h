/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 11:11:42 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 13:32:41 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# define MAX_HEREDOCS 16
# include "expander.h" // for t_expand_data

int		is_redirection(const char *token);
void	open_and_redirect_input(char *input_file);
int		process_output_file(char *output_file, int is_append);
void	open_and_redirect_output(char *output_file, int output_mode);
void	redirect_io(char *input_file, char *output_file, int output_mode);
char	**handle_redirection(char **args, t_process_data *p_data, char **final_input_file,
			char **final_output_file, int *output_mode, char **heredoc_file);

// heredoc_helper.c
char *generate_unique_heredoc_file(int *out_fd);
void cleanup_heredoc(char *line, int fd, char *tmp_filename, 
						  char *actual_delimiter);
void handle_eof_warning(const char *actual_delimiter);
char *process_variable(char *line, size_t *i, char *result, t_expand_data *data);
char *expand_heredoc_line(char *line, t_expand_data *data);

// heredoc.c
char	*handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status);

#endif