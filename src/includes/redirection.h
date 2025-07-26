/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.h                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 11:11:42 by tiyang        #+#    #+#                 */
/*   Updated: 2025/07/24 12:02:40 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REDIRECTION_H
# define REDIRECTION_H

// utils:
int		process_output_file(char *output_file);
void	open_and_redirect_input(char *input_file);
void	open_and_redirect_output(char *output_file);
int		count_clean_args(char **args);
int		is_redirection(const char *token);

// main functions:
void	redirect_io(char *input_file, char *output_file);
char	**handle_redirection(char **args, char **final_input_file, char **final_output_file);

#endif