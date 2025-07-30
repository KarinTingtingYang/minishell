/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:25:34 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 10:30:46 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/**
 * @brief Handles I/O redirection in the child process before command execution.
 *
 * @param input_file The file to redirect standard input from.
 * @param output_file The file to redirect standard output to.
 *
 * This function is called in the child process. It opens the specified files
 * and uses dup2 to replace STDIN_FILENO and STDOUT_FILENO.
 */
// Main redirection function
void	redirect_io(char *input_file, char *output_file, int output_mode)
{
	if (input_file)
		open_and_redirect_input(input_file);
	if (output_file)
		open_and_redirect_output(output_file, output_mode);
}

// handle_redirection() Helper: Process a single redirection and update input/output file pointers
static int	process_redirection_token(char **args, int i, 
								char **final_input_file, char **final_output_file, int *output_mode)
{
    if (!args[i + 1])
    {
        ft_putstr_fd("minishell: syntax error near `newline'\n", 2);
        return (-1);
    }
    if (ft_strncmp(args[i], "<", 2) == 0)
    {
        free(*final_input_file);
        *final_input_file = ft_strdup(args[i + 1]);
    }
    else if (ft_strncmp(args[i], ">", 2) == 0)
    {
		if (process_output_file(args[i + 1], 0) != 0) // 0 = truncate
            return (-1);
        free(*final_output_file);
        *final_output_file = ft_strdup(args[i + 1]);
		*output_mode = 1; // 1 = O_TRUNC
    }
	else if (ft_strncmp(args[i], ">>", 3) == 0)
	{
		if (process_output_file(args[i + 1], 1) != 0) // 1 = append
			return (-1);
		free(*final_output_file);
		*final_output_file = ft_strdup(args[i + 1]);
		*output_mode = 2; // 2 = O_APPEND
	}
    return (0);
}

// handle_redirection() Helper: Build clean argument array
static char	**build_clean_args(char **args, int argc)
{
    char	**clean_args = (char **)malloc(sizeof(char *) * (argc + 1));
    int		i = 0, j = 0;
    if (!clean_args)
        error_exit("malloc");
    while (args[i])
    {
        if (is_redirection(args[i]))
            i += 2;
        else
            clean_args[j++] = ft_strdup(args[i++]);
    }
    clean_args[j] = NULL;
    return clean_args;
}

/**
 * @brief Parses args for multiple redirections and returns a clean command array.
 * It finds the *last* input and *last* output file for actual redirection,
 * but processes *all* output files to create/truncate them.
 * @return A new, null-terminated array of strings containing only the command
 * and its arguments. Returns NULL on syntax or file error.
 */
// Main function split into helpers
char	**handle_redirection(char **args, char **final_input_file, char **final_output_file,
							int *output_mode)
{
    int		i = 0;
    int		argc;

    *final_input_file = NULL;
    *final_output_file = NULL;
	*output_mode = 0; // Initialize output_mode
    // First pass: process redirections and count clean args
    argc = 0;
    while (args[i])
    {
        if (is_redirection(args[i]))
        {
            if (process_redirection_token(args, i, final_input_file, final_output_file,
											output_mode) != 0)
                return NULL;
            i += 2;
        }
        else
        {
            argc++;
            i++;
        }
    }
    // Second pass: build clean argument array
    return build_clean_args(args, argc);
}