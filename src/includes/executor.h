/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiyang <tiyang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:39:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/06 15:07:09 by tiyang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

typedef struct s_command	t_command;

typedef struct s_env_var	t_env_var;

typedef struct s_execute_data
{
	char		**original_args;
	char		**clean_args;
	char		*input_file;
	char		*output_file;
	char		**path_dirs;
	char		*cmd_path;
	int			output_mode;
	t_env_var	*env_list;
	char		*heredoc_file;
}	t_execute_data;

// precheck_line.c
int			precheck_line(char *line, t_process_data *process_data);

// precheck_line_helper.c
int			check_redir_error(const char *line, int op_len, int j,
				t_process_data *process_data);

// path.c
char		*find_full_cmd_path(char *cmd, char **path_dirs);
char		**find_path_dirs(t_env_var *env_list);

// executor.c
int			perform_command_checks(char *cmd_path, char **args, char **envp);
void		execute_cmd(char *cmd_path, char **args, char **path_dirs,
				t_env_var *env_list);
int			handle_pipeline_command(char *line, t_env_var *env_list,
				t_process_data *process_data);
int			handle_single_command(char *line, t_env_var *env_list,
				t_process_data *process_data);
int			execute_command(char *line, t_env_var *env_list,
				t_process_data *process_data);

// execute_command_types.c
int			execute_builtin_command(t_execute_data *data,
				t_process_data *process_data);
int			execute_external_command(t_execute_data *data,
				t_process_data *process_data, t_env_var *env_list);
int			handle_redirection_only(t_execute_data *data,
				t_process_data *process_data);
int			execute_single_command(char **args, t_env_var *env_list,
				t_process_data *process_data);

// prepare_command_execution.c
int			prepare_command_execution(char *line, t_env_var *env_list,
				t_execute_data *data, t_process_data *process_data);

// execute_prepared_command.c
int			execute_prepared_command(t_execute_data *data,
				t_process_data *process_data);

// prepare_pipeline_helper.c
int			build_commands_from_parts(char **parts, int count,
				t_process_data *data);

// prepare_pipeline_commands.c
t_command	**prepare_pipeline_commands(char *line, int *count, char ***parts,
				t_process_data *process_data);

// executor_helper.c
int			validate_pipeline_parts(char **parts, int count);
int			is_empty_or_whitespace(const char *str);
char		**ft_split_dup(char **args);
int			count_command_parts(char **parts);
int			is_unquoted_pipe_present(const char *line);

// executor_error.c
void		handle_stat_error(char **envp, char **args, int error_code);
void		handle_access_error(char **envp, char **args, int error_code);
void		handle_execve_error(char **envp, char **args, int error_code);
void		handle_null_cmd_path(char **args, char **envp);
int			handle_redirection_error(t_execute_data *data,
				t_process_data *process_data, char **args);

// executor_cleanup.c
void		free_execute_data(t_execute_data *data);
void		free_commands_recursive(t_command **cmds, int index, int count);
// void		cleanup_pipeline_resources(t_command **cmds, char **parts,
// 				char **path_dirs, int count);
void		cleanup_pipeline_resources(t_process_data *data);
void		cleanup_child_pipeline_resources(t_process_data *data);

// execute_builtin_command.c 
void		restore_builtin_io(int did_save, int saved_stdin, int saved_stdout);
int			setup_builtin_io(t_execute_data *data,
				int *saved_stdin, int *saved_stdout, int *did_save);
#endif