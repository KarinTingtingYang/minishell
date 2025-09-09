/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipes.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/22 11:30:32 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/09 13:17:28 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

typedef struct s_env_var	t_env_var;

/** @brief Struct to represent a command with its arguments and redirections.
 *
 * This struct holds all necessary information for executing a command,
 * including the command path, arguments, input/output redirection files,
 * output mode (append or truncate), and heredoc file if applicable.
 */
typedef struct s_command
{
	char	*cmd_path;
	char	**args;
	char	*input_file;
	char	*output_file;
	int		output_mode;
	char	*heredoc_file;
}	t_command;

/**
 * @brief Struct to hold all necessary data for process execution.
 *
 * This struct contains information about the commands to execute,
 * pipe file descriptors, environment variables, process IDs, and
 * execution state such as the last exit status and syntax error flag.
 */
typedef struct s_process_data
{
	t_command	**cmds;
	int			**pipes;
	char		**path_dirs;
	pid_t		*pids;
	int			cmd_count;
	t_env_var	*env_list;
	int			last_exit_status;
	int			in_pipeline;
	int			syntax_error;
	char		**parts;
}	t_process_data;

/**
 * @brief Struct to hold state information for child process waiting.
 *
 * This struct is used to pass multiple state variables to the recursive
 * function as a single argument, reducing the argument count.
 */
typedef struct s_wait_info
{
	int	last_status;
	int	signal_printed;
}	t_wait_info;

pid_t	*create_child_processes(int cmd_count, int **pipes);
int		run_pipeline_core(t_process_data *data, int cmd_count);
int		fork_all_processes_recursive(t_process_data *data, int i);
void	execute_child_command(t_command *cmd, t_process_data *data);
void	close_free_pipes_recursively(int **pipes, int idx, int max);
int		create_all_pipes_recursively(int **pipes, int index, int max);
int		run_command_pipeline(t_process_data *data);
void	handle_builtin_command(t_command *cmd, t_process_data *data);
void	handle_path_resolution_error(char *cmd_name, char **envp,
			t_process_data *data);
void	handle_stat_error_child(char *cmd_name, char **envp,
			t_process_data *data, int error_code);
void	handle_access_error_child(char *cmd_name, char **envp,
			t_process_data *data, int error_code);
void	handle_execve_error_child(char *cmd_name, char **envp,
			t_process_data *data, int error_code);

#endif