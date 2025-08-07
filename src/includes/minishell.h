/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:10:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 14:12:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L   // Added for signal handling
# define _GNU_SOURCE               // Added for signal handling

# include <signal.h>				// Added for signal handling
# include <stdint.h>                // Added for signal handling
# include <sys/types.h>				// Added for signal handling
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

#include "./libft.h"
#include "./pipes.h"
#include "./parser.h"
#include "./executor.h"
#include "./builtins.h"
#include "./redirection.h"
#include "./signal_handling.h"
#include "./lexer.h"
#include "./env.h"
#include "./expander.h"


// Global flag to indicate if a child process is currently running.
// volatile sig_atomic_t is used for variables modified by signal handlers.
// extern volatile sig_atomic_t g_child_running;

// NEW GLOBAL VARIABLE FOR SIGNAL HANDLING INCLUDING HEREDOC
extern volatile sig_atomic_t g_signal_received;

void		free_split(char **array);
void		error_exit(const char *msg);
void		error_msg_exit(const char *msg);
t_env_var	*init_env(char **environ);
void		free_env(t_env_var *env);
char		*ft_strjoin_free(char *s1, char *s2);
// void print_array(char **array);
// void print_tokens(t_token **tokens);

#endif