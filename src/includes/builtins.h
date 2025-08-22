/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 09:01:24 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/22 09:41:50 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef BUILTINS_H
# define BUILTINS_H

int		apply_builtin_redirection(char *input_file, char *output_file, int output_mode);
int		is_builtin(char *cmd);
//int		run_builtin(char **args, t_env_var *env_list);
// EXIT CODE DEBUG
// the exit built in needs to receive the exit code from last execution
int		run_builtin(char **args, t_process_data *process_data); 

// Function prototypes for built-in commands
// TO DO: THE RETURN VALUES OF BUILTINS ARE NOT CONSISTENT 
// (CD AND PWD RETURN 0 ON SUCCESS, ECHO BUILTIN RETURNS NOTHING)

int		run_pwd(void);
int		run_cd(char **args, t_env_var *env_list);
// void	run_echo(char **args);
void run_echo(char **args, t_env_var *env_list);
int		run_exit(char **args, int last_exit_status);
void 	run_env(t_env_var *env_list);
int 	run_export(t_env_var *env_list, char **args);
int		is_valid_identifier(const char *str);
int		run_export(t_env_var *env_list, char **args);
int		run_unset(t_env_var **env_list, char **args);

#endif