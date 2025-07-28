/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:01:24 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/28 12:29:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

// I/O handling for built-ins
int		apply_builtin_redirection(char *input_file, char *output_file);



int		is_builtin(char *cmd);
int		run_builtin(char **args);

// Function prototypes for built-in commands
// TO DO: THE RETURN VALUES OF BUILTINS ARE NOT CONSISTENT 
// (CD AND PWD RETURN 0 ON SUCCESS, ECHO BUILTIN RETURNS NOTHING)

int	builtin_pwd(void);
int	builtin_cd(char **args);
void	run_echo(char **args);

#endif
