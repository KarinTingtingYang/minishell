/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:10:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 10:02:30 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../libft/libft.h"
#include "../src/includes/executor.h"

void	free_split(char **array);
void	error_exit(const char *msg);
void	error_msg_exit(const char *msg);
char	*find_full_cmd_path(char *cmd, char **path_dirs);
void	run_last_child(int pipe_fd[2], char **argv, char **envp);
void	run_first_child(int pipe_fd[2], char **argv, char **envp);

#endif