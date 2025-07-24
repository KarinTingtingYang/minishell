/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:02:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 09:20:24 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	error_msg_exit(const char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	free_split(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL; // added safeguard to avoid double free issues
		i++;
	}
	free(array);
	array = NULL;  // added safeguard to avoid double free issues
}

// REDIRECTION: Modified free_command to also free redirection file strings.
void free_command(t_command *cmd)
{
    if (!cmd)
        return;
    free_split(cmd->args);
	if (cmd->cmd_path)
        free(cmd->cmd_path);

	if (cmd->input_file) // Free the memory for input/output files.
		free(cmd->input_file);
	if (cmd->output_file) // Free the memory for input/output files.
		free(cmd->output_file);
	free(cmd);
}

// REDIRECTION: Modified create_command to parse for IO redirection.
t_command *create_command(char *line, char **path_dirs)
{
    t_command *cmd;
    // char **args;
	char **original_args; // REDIRECTION: remaning "args" to "original_args" for clarity
	char **clean_args; // REDIRECTION: new variable to hold args without redirection signs

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;

		    // Initialize struct fields to NULL
    cmd->args = NULL;
    cmd->cmd_path = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
	
    original_args = ft_split(line, ' ');
    if (!original_args || !original_args[0])
    {
        free_split(original_args);
        free(cmd);
        return NULL;
    }

	/* MAIN REDIRECTION LOGIC STARTS */
	
	// REDIRECTION: Call handle_redirection to parse for '<' and '>'.
    // It returns a new array of "clean" arguments.
	clean_args = handle_redirection(original_args, &cmd->input_file, &cmd->output_file);
	// REDIRECTION: The original split is no longer needed after parsing.
    free_split(original_args);
    if (clean_args == NULL) // Indicates a syntax error
    {
       // free_split(original_args);
        free(cmd);
        return (NULL);
    }
	// REDIRECTION: Handle cases like "> out" where no command is left.
    if (clean_args[0] == NULL)
    {
        cmd->args = clean_args;
        cmd->cmd_path = NULL;
        cmd->input_file = NULL;
        cmd->output_file = NULL;
     //   free_split(original_args);
        return (cmd);
    }
    //cmd->args = args;
	cmd->args = clean_args; // REDIRECTION: proceed with cleaned up args

	/* MAIN REDIRECTION LOGIC ENDS */

	// REDIRECTION: Find command path using the first of the *clean* arguments.
    cmd->cmd_path = find_full_cmd_path(cmd->args[0], path_dirs);
	

    if (!cmd->cmd_path)
    {
        // free_command should free args and cmd
        free_command(cmd);
        return NULL;
    }
    return cmd;
}

