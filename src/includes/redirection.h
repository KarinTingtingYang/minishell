/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:11:42 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/30 10:30:22 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	REDIRECTION_H
# define REDIRECTION_H

// utils:
int	process_output_file(char *output_file, int is_append);
void	open_and_redirect_input(char *input_file);
void	open_and_redirect_output(char *output_file, int output_mode);
int		count_clean_args(char **args);
int		is_redirection(const char *token);

// main functions:
void	redirect_io(char *input_file, char *output_file, int output_mode);
char	**handle_redirection(char **args, char **final_input_file, 
	char **final_output_file, int *output_mode);

#endif