/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:30:32 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:20:22 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPES_H
# define PIPES_H

typedef struct s_env_var	t_env_var;

typedef struct s_command
{
	char	*cmd_path;
	char	**args;
	char	*input_file;
	char	*output_file;
	int		output_mode;
	char	*heredoc_file;
}	t_command;

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
}	t_process_data;

int		**prepare_pipe_fds(int cmd_count);
int		**create_pipe_fds_between_commands(int cmd_count);
pid_t	*create_child_processes(int cmd_count, int **pipes);
void	close_free_pipes_recursively(int **pipes, int idx, int max);
int		create_all_pipes_recursively(int **pipes, int index, int max);
int		run_command_pipeline(t_command **cmds, int cmd_count, char **path_dirs,
			t_env_var *env_list);

#endif