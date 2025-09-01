/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mariahudonogova <mariahudonogova@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:39:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/02 01:17:27 by mariahudono      ###   ########.fr       */
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
	t_env_var *env_list;
	char *heredoc_file;    // File name for heredoc redirection (if any)
	
} t_execute_data;

// precheck_line.c
int precheck_line(char *line, t_process_data *process_data);

// path.c
char *find_full_cmd_path(char *cmd, char **path_dirs);
char **find_path_dirs(t_env_var *env_list);

// executor.c
int perform_command_checks(char *cmd_path, char **args, char **envp);
void execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list);
int handle_pipeline_command(char *line, t_env_var *env_list, t_process_data *process_data);
int handle_single_command(char *line, t_env_var *env_list, t_process_data *process_data);
int execute_command(char *line, t_env_var *env_list, t_process_data *process_data);

// execute_command_types.c
int execute_builtin_command(t_execute_data *data, t_process_data *process_data);
int execute_external_command(t_execute_data *data, t_process_data *process_data, t_env_var *env_list);
int handle_redirection_only(t_execute_data *data, t_process_data *process_data);
int execute_single_command(char **args, t_env_var *env_list, t_process_data *process_data);

// prepare_command_execution.c
int prepare_command_execution(char *line, t_env_var *env_list, t_execute_data *data,
								t_process_data *process_data);

// execute_prepared_command.c
int execute_prepared_command(t_execute_data *data, t_process_data *process_data);

// prepare_pipeline_commands.c
int build_commands_from_parts(t_command **cmds, char **parts, int index, int count,
							  t_process_data *process_data);
t_command **prepare_pipeline_commands(char *line, int *count, char ***parts,
                                      t_process_data *process_data);

// executor_helper.c
int		validate_pipeline_parts(char **parts, int count);
int		is_empty_or_whitespace(const char *str);
char	**ft_split_dup(char **args);
int		count_command_parts(char **parts);
int 	check_heredoc_limit(char *line);
int		is_unquoted_pipe_present(const char *line);

// executor_error.c
void handle_stat_error(char **envp, char **args, int error_code);
void handle_access_error(char **envp, char **args, int error_code);
void handle_execve_error(char **envp, char **args, int error_code);
void handle_null_cmd_path(char **args, char **envp);
int handle_redirection_error(t_execute_data *data, t_process_data *process_data,
							char **args);

// executor_cleanup.c
void free_execute_data(t_execute_data *data);
void free_commands_recursive(t_command **cmds, int index, int count);

#endif