/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtins.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/28 09:01:24 by makhudon      #+#    #+#                 */
/*   Updated: 2025/07/31 09:40:02 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#ifndef BUILTINS_H
# define BUILTINS_H

typedef struct s_env_var // Structure to represent an environment variable
{
    char            *key;       // Variable name (key)
    char            *value;     // Variable value
    struct s_env_var *next; 	// Pointer to the next environment variable in the linked list
}   t_env_var;

// I/O handling for built-ins
// APPEND REDIRECTION: ADDED FLAG FOR OUTPUT MODE 0=none, 1=truncate(>), 2=append(>>)
int		apply_builtin_redirection(char *input_file, char *output_file, int output_mode);



int		is_builtin(char *cmd);
int		run_builtin(char **args, t_env_var *env_list);

// Function prototypes for built-in commands
// TO DO: THE RETURN VALUES OF BUILTINS ARE NOT CONSISTENT 
// (CD AND PWD RETURN 0 ON SUCCESS, ECHO BUILTIN RETURNS NOTHING)

int	builtin_pwd(void);
int	builtin_cd(char **args);
void	run_echo(char **args);
int		builtin_exit(char **args);
void builtin_env(t_env_var *env_list);
int run_export(t_env_var *env_list, char **args);
char *get_env_value(t_env_var *env, char *key);
char **env_list_to_array(t_env_var *env);

#endif