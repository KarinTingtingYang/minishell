/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 11:30:32 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/22 11:46:44 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_UTILS_H
#define PIPEX_UTILS_H

// Example command structure for the pipeline
typedef struct s_command
{
    char    *cmd_path;  // full path to executable (resolved)
    char    **args;     // NULL-terminated array of arguments (argv)
}   t_command;

// Prototype of the main function you'll use to execute pipelines
int execute_pipeline(t_command **cmds, int cmd_count, char **path_dirs);
t_command *create_command(char *line, char **envp);
void free_command(t_command *cmd);

#endif