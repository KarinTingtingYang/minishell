/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:39:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 09:48:17 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

typedef struct s_command t_command;

typedef struct s_execute_data
{
    char **original_args;  // The raw split arguments from the command line
    char **clean_args;     // The arguments after removing redirection symbols and files
    char *input_file;      // File name for input redirection (if any)
    char *output_file;     // File name for output redirection (if any)
    char **path_dirs;      // Array of directories in PATH environment variable
    char *cmd_path;        // Full path to the executable command
} t_execute_data;


int			execute_command(char *line, char **envp);
int			count_command_parts(char **parts);
void		free_commands_recursive(t_command **cmds, int index, int count);
char		**find_path_dirs(char **envp);
char		*find_full_cmd_path(char *cmd, char **path_dirs);
t_command	**prepare_pipeline_commands(char *line, int *count, char ***parts, char **envp);
int			prepare_command_execution(char *line, char **envp, t_execute_data *data);
int			execute_prepared_command(t_execute_data *data);
void		free_execute_data(t_execute_data *data);
void		execute_cmd(char *cmd_path, char **args, char **path_dirs);

#endif
