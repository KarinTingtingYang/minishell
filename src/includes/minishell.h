/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:10:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 08:44:04 by makhudon         ###   ########.fr       */
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

// Global flag to indicate if a child process is currently running.
// volatile sig_atomic_t is used for variables modified by signal handlers.
extern volatile sig_atomic_t g_child_running;

void		free_split(char **array);
void		error_exit(const char *msg);
void		error_msg_exit(const char *msg);

#endif