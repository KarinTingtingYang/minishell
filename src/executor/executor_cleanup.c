/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor_cleanup.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 08:58:09 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/03 11:47:36 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_execute_data(t_execute_data *data)
{
    if (data == NULL)
        return ;
    if (data->heredoc_file != NULL)
	{
		unlink(data->heredoc_file);
		free(data->heredoc_file);
	}
    free(data->input_file);
    free(data->output_file);
    free_split(data->path_dirs);
    free_split(data->original_args);
    free_split(data->clean_args);
    free(data->cmd_path);
    data->path_dirs = NULL;
    data->original_args = NULL;
    data->clean_args = NULL;
    data->cmd_path = NULL;
    data->heredoc_file = NULL;
    data->input_file = NULL;
    data->output_file = NULL;
}

void free_commands_recursive(t_command **cmds, int index, int count)
{
    if (index >= count)
        return ;
    free_command(cmds[index]);
    free_commands_recursive(cmds, index + 1, count);
}

void cleanup_pipeline_resources(t_command **cmds, char **parts, 
                                     char **path_dirs, int count)
{
    if (path_dirs)
        free_split(path_dirs);
    free_commands_recursive(cmds, 0, count);
    free(cmds);
    free_split(parts);
}
// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------
// int precheck_pipe_syntax(const char *line)
// {
//     int   i;
//     char  quote;      /* current active quote char (' or ") or 0 if none */
//     int   seen_token; /* have we seen a non-space token since last '|' ? */

//     i = 0;
//     quote = 0;
//     seen_token = 0;
//     while (line && line[i])
//     {
//         if ((line[i] == '\'' || line[i] == '"') && quote == 0)
//         {
//             quote = line[i];
//             i++;
//             continue;
//         }
//         if (quote && line[i] == quote)
//         {
//             quote = 0;
//             i++;
//             seen_token = 1;
//             continue;
//         }
//         if (quote == 0 && line[i] == '|')
//         {
//             int j;

//             /* leading '|', double '||', or trailing '|' */
//             j = i + 1;
//             while (line[j] == ' ' || line[j] == '\t')
//                 j++;
//             if (seen_token == 0 || line[j] == '|' || line[j] == '\0')
//             {
//                 ft_error(NULL, "syntax error near unexpected token `|'");
//                 return (0);
//             }
//             seen_token = 0; /* expect a token after this pipe */
//             i++;
//             continue;
//         }
//         if (quote == 0 && line[i] != ' ' && line[i] != '\t')
//             seen_token = 1;
//         i++;
//     }
//     return (1);
// }
