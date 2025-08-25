/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 11:11:42 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/25 10:04:07 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef	REDIRECTION_H
# define REDIRECTION_H

# define MAX_HEREDOCS 16           // Maximum number of here-documents

// utils:
int	process_output_file(char *output_file, int is_append);
void	open_and_redirect_input(char *input_file);
void	open_and_redirect_output(char *output_file, int output_mode);
int		count_clean_args(char **args);
int		is_redirection(const char *token);

// heredoc:
char *handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status);


// main functions:
void	redirect_io(char *input_file, char *output_file, int output_mode);
// int redirect_io(char *input_file, char *output_file, int output_mode);
// int open_and_redirect_input(const char *input_file);
// int open_and_redirect_output(const char *output_file, int output_mode);
char	**handle_redirection(char **args, char **final_input_file, char **final_output_file,
							int *output_mode, char **heredoc_file, t_env_var *env_list, int last_exit_status);

#endif