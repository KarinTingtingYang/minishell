/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_pipeline_commands.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/30 16:42:20 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 14:31:57 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Allocates memory for the command array.
 *
 * @param count The number of commands.
 * @param cmds Pointer to the command array.
 * @return 1 on success, 0 on failure.
 */
static int allocate_command_array(int count, t_command ***cmds)
{
    *cmds = malloc(sizeof(t_command *) * ((size_t)count + 1));
    if (*cmds == NULL)
        return (0);
    ft_bzero(*cmds, sizeof(t_command *) * ((size_t)count + 1));
    return (1);
}

/**
 * @brief Validates the command line and allocates necessary structures.
 *
 * This function performs a precheck on the command line, splits it by pipes,
 * counts the number of command parts, and validates the syntax of the pipeline.
 * If any step fails, it cleans up allocated resources and returns 0.
 *
 * @param line The command line to validate.
 * @param count Pointer to store the number of command parts.
 * @param parts Pointer to store the split command parts.
 * @param data Pointer to the process data structure for syntax error tracking.
 * @return 1 on success, 0 on failure.
 */
static int validate_and_allocate(char *line, int *count, char ***parts,
                               t_process_data *data)
{
    if (!precheck_line(line, data))
        return (0);
    *parts = split_line_by_pipe(line);
    if (*parts == NULL || (*parts)[0] == NULL)
        return (0);
    *count = count_command_parts(*parts);
    if (data)
        data->syntax_error = 0;
    if (!validate_pipeline_parts(*parts, *count))
    {
        free_split(*parts);
        *parts = NULL;
        return (0);
    }
    return (1);
}

/**
 * @brief Prepares the pipeline commands.
 *
 * This function validates the command line, splits it by pipes,
 * allocates the command array, and builds the command structures
 * from the split parts. It returns the command array on success
 * or NULL on failure.
 *
 * @param line The command line.
 * @param count Pointer to the number of commands.
 * @param parts Pointer to the array of command parts.
 * @param data Pointer to the process data.
 * @return Pointer to the command array, or NULL on failure.
 */
t_command **prepare_pipeline_commands(char *line, int *count, char ***parts,
                                    t_process_data *data)
{
    if (!validate_and_allocate(line, count, parts, data))
        return (NULL);
    if (!allocate_command_array(*count, &data->cmds))
    {
        free_split(*parts);
        return (NULL);
    }
    if (!build_commands_from_parts(*parts, *count, data))
    {
        free_commands_recursive(data->cmds, 0, *count);
        free(data->cmds);
        free_split(*parts);
        return (NULL);
    }
    data->cmds[*count] = NULL;
    return (data->cmds);
}

// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------

// /**
//  * @brief Allocates memory for the command array.
//  *
//  * @param count The number of commands.
//  * @param parts Array of command parts.
//  * @param cmds Pointer to the command array.
//  * @return 1 on success, 0 on failure.
//  */
// static int allocate_command_array(int count, char ***parts, t_command ***cmds)
// {
//     *cmds = malloc(sizeof(t_command *) * ((size_t)count + 1));
//     if (*cmds == NULL)
//     {
//         free_split(*parts);
//         *parts = NULL;
//         return (0);
//     }
//     ft_bzero(*cmds, sizeof(t_command *) * ((size_t)count + 1));
//     return (1);
// }


// /**
//  * @brief Creates a command structure.
//  *
//  * @param expanded_args The expanded arguments.
//  * @param path_dirs The path directories.
//  * @param process_data Pointer to the process data.
//  * @param cmds Array of command structures.
//  * @param index Index of the command structure.
//  * @return 1 on success, 0 on failure.
//  */
// static int create_command_structure(char **expanded_args, char **path_dirs,
//                                    t_process_data *process_data, t_command **cmds, int index)
// {
//     cmds[index] = create_command(expanded_args, path_dirs, process_data);
//     free_split(expanded_args);
//     if (cmds[index] == NULL)
//     {
//         if (g_signal_received == SIGINT)
//             return (0);
//         ft_error(NULL, "command creation failed");
//         return 0;
//     }
//     return 1;
// }

// /**
//  * @brief Parses the line and expands the arguments.
//  *
//  * @param parts Array of command parts.
//  * @param process_data Pointer to the process data.
//  * @param path_dirs The path directories.
//  * @param cmds Array of command structures.
//  * @param index Index of the command structure.
//  * @return 1 on success, 0 on failure.
//  */
// static int parse_and_expand(char **parts, t_process_data *process_data, char **path_dirs,
//                             t_command **cmds, int index)
// {
//     t_token **tokens = parse_line(parts[index]);
//     char **expanded_args;

//     if (tokens == NULL)
//         return 0;

//     expanded_args = expand_and_split_args(tokens,
//                                           process_data->env_list,
//                                           process_data->last_exit_status);
//     free_tokens(tokens);
//     if (expanded_args == NULL)
//         return 0;

//     if (!create_command_structure(expanded_args, path_dirs, process_data, cmds, index))
//         return 0;

//     return 1;
// }

// /**
//  * @brief Builds command structures from split command parts.
//  *
//  * @param cmds Pointer to an array of `t_command*` where commands will be stored.
//  * @param parts Array of command parts split by pipes.
//  * @param index Current index in the parts array.
//  * @param count Total number of command parts.
//  * @param process_data Pointer to the process data.
//  * @return Returns 1 on success, or 0 if an error occurs.
//  */
// int build_commands_from_parts(t_command **cmds, char **parts, int index, int count,
//                               t_process_data *process_data)
// {
//     char **path_dirs;

//     path_dirs = find_path_dirs(process_data->env_list);
//     if (path_dirs == NULL)
//     {
//         ft_error(NULL, "PATH variable not found");
//         return 0;
//     }

//     process_data->in_pipeline = (count > 1);

//     while (index < count)
//     {
//         if (!parse_and_expand(parts, process_data, path_dirs, cmds, index))
//         {
//             free_split(path_dirs);
//             return 0;
//         }
//         index++;
//     }

//     free_split(path_dirs);
//     return 1;
// }

// /**
//  * @brief Prepares the pipeline commands.
//  *
//  * @param line The command line.
//  * @param count Pointer to the number of commands.
//  * @param parts Pointer to the array of command parts.
//  * @param process_data Pointer to the process data.
//  * @return Pointer to the command array.
//  */
// t_command **prepare_pipeline_commands(char *line, int *count, char ***parts,
//                                       t_process_data *process_data)
// {
//     t_command **cmds;

//     if (!precheck_line(line, process_data))
//         return NULL;
//     *parts = split_line_by_pipe(line);
//     if (*parts == NULL || (*parts)[0] == NULL)
//         return (NULL);
//     *count = count_command_parts(*parts);
//     if (process_data)
//         process_data->syntax_error = 0;
//     if (!validate_pipeline_parts(*parts, *count))
// 		return (free_split(*parts), *parts = NULL, NULL);
//     if (!allocate_command_array(*count, parts, &cmds))
//         return (NULL);
//     if (process_data)
//         process_data->in_pipeline = (*count > 1);
//     if (!build_commands_from_parts(cmds, *parts, 0, *count, process_data))
//     {
//         free_commands_recursive(cmds, 0, *count);
//         free(cmds);
// 		return (free_split(*parts), *parts = NULL, NULL);
// 	}
// 	cmds[*count] = NULL;
// 	return cmds;
// }