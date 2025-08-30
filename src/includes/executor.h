/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:39:33 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 13:58:49 by makhudon         ###   ########.fr       */
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

char		**find_path_dirs(t_env_var *env_list);
int			count_command_parts(char **parts);
void		free_execute_data(t_execute_data *data);
int			execute_command(char *line, t_env_var *env_list, t_process_data *process_data);
int			execute_prepared_command(t_execute_data *data, t_process_data *process_data);
char		*find_full_cmd_path(char *cmd, char **path_dirs);
void		execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list);
void		free_commands_recursive(t_command **cmds, int index, int count);
int			prepare_command_execution(char *line, t_env_var *env_list, t_execute_data *data, t_process_data *process_data);
t_command	**prepare_pipeline_commands(char *line, int *count, char ***parts, t_process_data *process_data);
char		**ft_split_dup(char **args);

#endif