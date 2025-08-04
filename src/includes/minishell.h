/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/04 13:10:15 by makhudon      #+#    #+#                 */
/*   Updated: 2025/07/31 11:54:48 by tiyang        ########   odam.nl         */
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
#include "./parse.h"
#include "./executor.h"
#include "./builtins.h"
#include "./redirection.h"
#include "./signal_handling.h"
#include "./lexer.h"
#include "./env.h"


// typedef struct s_env_var // Structure to represent an environment variable
// {
//     char            *key;       // Variable name (key)
//     char            *value;     // Variable value
//     struct s_env_var *next; 	// Pointer to the next environment variable in the linked list
// }   t_env_var;

// Global flag to indicate if a child process is currently running.
// volatile sig_atomic_t is used for variables modified by signal handlers.
extern volatile sig_atomic_t g_child_running;

void		free_split(char **array);
void		error_exit(const char *msg);
void		error_msg_exit(const char *msg);
t_env_var	*init_env(char **environ);
void		free_env(t_env_var *env);

#endif