/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:30:32 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 13:05:36 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_UTILS_H
#define PIPEX_UTILS_H

typedef struct s_command
{
    char	*cmd_path;     // full path to executable (resolved)
    char	**args;        // NULL-terminated array of arguments (argv)
    char	*input_file;
    char	*output_file;
}   t_command;

typedef struct s_process_data
{
    t_command	**cmds;      // Array of pointers to commands to execute; each command includes path, args, and redirection info
    int			**pipes;     // 2D array of pipe file descriptors for inter-process communication between commands
    char		**path_dirs; // Array of directories from the PATH environment variable to search for executables
    pid_t		*pids;       // Array of process IDs for the forked child processes
    int			cmd_count;   // Total number of commands in the pipeline to execute
} t_process_data;

int		**prepare_pipe_fds(int cmd_count);
int		**create_pipe_fds_between_commands(int cmd_count);
pid_t	*create_child_processes(int cmd_count, int **pipes);
void	close_free_pipes_recursively(int **pipes, int idx, int max);
int		create_all_pipes_recursively(int **pipes, int index, int max);
int		run_command_pipeline(t_command **cmds, int cmd_count, char **path_dirs);

#endif