/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:39:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 13:32:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

typedef struct s_command t_command;

typedef struct s_env_var t_env_var;

typedef struct s_execute_data
{
    char **original_args;  // The raw split arguments from the command line
    char **clean_args;     // The arguments after removing redirection symbols and files
    char *input_file;      // File name for input redirection (if any)
    char *output_file;     // File name for output redirection (if any)
    char **path_dirs;      // Array of directories in PATH environment variable
    char *cmd_path;        // Full path to the executable command
	int  output_mode;		//  FOR APPEND REDIRECTION: 0=none, 1=truncate(>), 2=append(>>)
	// char **envp;	   // passing envp down to built-in commands
	t_env_var *env_list;
} t_execute_data;

char		**find_path_dirs(t_env_var *env_list);
int			count_command_parts(char **parts);
void		free_execute_data(t_execute_data *data);
int execute_command(char *line, t_env_var *env_list);
int			execute_prepared_command(t_execute_data *data);
char		*find_full_cmd_path(char *cmd, char **path_dirs);
void		execute_cmd(char *cmd_path, char **args, char **path_dirs);
void		free_commands_recursive(t_command **cmds, int index, int count);
int			prepare_command_execution(char *line, t_env_var *env_list, t_execute_data *data);
t_command	**prepare_pipeline_commands(char *line, int *count, char ***parts, t_env_var *env_list);

#endif