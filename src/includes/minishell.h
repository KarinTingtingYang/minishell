/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:10:15 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:25:24 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _POSIX_C_SOURCE 200809L
# define _GNU_SOURCE

// #ifndef WCOREDUMP  // Some systems may not define WCOREDUMP
// #  define WCOREDUMP(x) 0 // Define it as 0 if not available
// #endif

# include <signal.h>
# include <stdint.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/history.h>
# include <readline/readline.h>

# include "./libft.h"
# include "./pipes.h"
# include "./parser.h"
# include "./executor.h"
# include "./builtins.h"
# include "./redirection.h"
# include "./signal_handling.h"
# include "./lexer.h"
# include "./env.h"
# include "./expander.h"

extern volatile sig_atomic_t	g_signal_received;

void	free_split(char **array);
void	skip_spaces(const char **s);
void	ft_error(char *command, char *message);
void	ft_error_with_arg(char *command, char *arg, char *message);
void	error_with_backticked_arg(const char *cmd, const char *arg,
			const char *msg);
void	ft_error_and_exit(char *command, char *message, int exit_status);

#endif