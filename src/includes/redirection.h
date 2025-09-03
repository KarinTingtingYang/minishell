/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:11:42 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/03 16:04:07 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H

# define MAX_HEREDOCS 16
# include "expander.h"

typedef struct s_heredoc_context
{
    int             fd;
    int             expand_content;
    t_expand_data   *data;
    char            *actual_delimiter;
    char            *tmp_filename;
} t_heredoc_context;

int		is_redirection(const char *token);
void	open_and_redirect_input(char *input_file);
int		process_output_file(char *output_file, int is_append);
void	open_and_redirect_output(char *output_file, int output_mode);
char	**handle_redirection(char **args, t_process_data *process_data,
			t_execute_data *exec_data);
void	redirect_io(char *input_file, char *output_file, int output_mode);
int		process_redirection_token(char **args, int i,
			t_process_data *process_data, t_execute_data *exec_data);

// heredoc_helper.c
char	*generate_unique_heredoc_file(int *out_fd);
void	handle_eof_warning(const char *actual_delimiter);
char	*expand_heredoc_line(char *line, t_expand_data *data);
char	*process_variable(char *line, size_t *i, char *result,
			t_expand_data *data);
void	cleanup_heredoc(char *line, int fd, char *tmp_filename,
			char *actual_delimiter);

// heredoc.c
char	*handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status);
int 	check_heredoc_limit(char *line);

#endif