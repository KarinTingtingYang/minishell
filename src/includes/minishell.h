/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:10:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/22 11:31:14 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L   // Added for signal handling
# define _GNU_SOURCE               // Added for signal handling

# include <fcntl.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>                // Added for signal handling

#include "../libft/libft.h"
#include "../src/includes/executor.h"
#include "../src/includes/pipex_utils.h"

// Global flag to indicate if a child process is currently running.
// volatile sig_atomic_t is used for variables modified by signal handlers.
extern volatile sig_atomic_t g_child_running; // Declare global flag

// Signal handling function prototypes (in signal.c)
void setup_signal_handlers(void);
void reset_child_signal_handlers(void);
void handle_parent_sigint(int signum);
int wait_for_child_and_handle_status(pid_t pid);

void error_exit(const char *msg);
void error_msg_exit(const char *msg);
void free_split(char **array);
char *find_full_cmd_path(char *cmd, char **path_dirs);
char **find_path_dirs(char **envp);
char **parse_line(char *line);
int		run_pipex(char *cmd1_line, char *cmd2_line, char **envp);
void handle_execve_error(char *cmd_path, char **args, char **path_dirs);
void execute_cmd(char *cmd_path, char **args, char **path_dirs);

#endif