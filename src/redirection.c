/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:25:34 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/24 08:25:43 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Processes an output file by creating/truncating it.
 * This mimics shell behavior where all output files in a command are created.
 */
// Helper function to handle the creation/truncation of ALL
// specified output files, not just the last one.
static int	process_output_file(char *output_file)
{
	int	fd;

	fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(output_file);
		return (-1);
	}
	close(fd);
	return (0);
}

/**
 * @brief Handles I/O redirection in the child process before command execution.
 *
 * @param input_file The file to redirect standard input from.
 * @param output_file The file to redirect standard output to.
 *
 * This function is called in the child process. It opens the specified files
 * and uses dup2 to replace STDIN_FILENO and STDOUT_FILENO.
 */
// New function to handle file descriptor redirection.
void	redirect_io(char *input_file, char *output_file)
{
	int	fd_in;
	int	fd_out;

	// Handle input redirection (<)
	if (input_file)
	{
		fd_in = open(input_file, O_RDONLY);
		if (fd_in == -1)
		{
			perror(input_file);
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd_in);
	}
	// Handle output redirection (>)
	if (output_file)
	{
		fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
		{
			perror(output_file);
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close(fd_out);
	}
}

/**
 * @brief Parses args for multiple redirections and returns a clean command array.
 * It finds the *last* input and *last* output file for actual redirection,
 * but processes *all* output files to create/truncate them.
 * @return A new, null-terminated array of strings containing only the command
 * and its arguments. Returns NULL on syntax or file error.
 */
// function to handle multiple redirections.
char	**handle_redirection(char **args, char **final_input_file, char **final_output_file)
{
	int		i;
	int		j;
	int		argc;
	char	**clean_args;

	i = 0;
	argc = 0;
	*final_input_file = NULL;
	*final_output_file = NULL;
	// First pass: Find redirections and count clean arguments.
	while (args[i])
	{
		if (ft_strncmp(args[i], "<", 2) == 0 || ft_strncmp(args[i], ">", 2) == 0)
		{
			if (!args[i + 1])
			{
				ft_putstr_fd("minishell: syntax error near `newline'\n", 2);
				return (NULL); // Syntax error
			}
			if (ft_strncmp(args[i], "<", 2) == 0)
			{
				free(*final_input_file); // Free previous one if any
				*final_input_file = ft_strdup(args[i + 1]);
			}
				
			if (ft_strncmp(args[i], ">", 2) == 0)
			{
				if (process_output_file(args[i + 1]) != 0)
					return (NULL); // Stop if a file can't be opened
				free(*final_output_file); // Free previous one if any
				*final_output_file = ft_strdup(args[i + 1]);
			}
			i += 2; // Skip operator and filename
		}
		else
		{
			argc++;
			i++;
		}
	}
	// Allocate memory for the new, cleaned argument list.
	clean_args = (char **)malloc(sizeof(char *) * (argc + 1));
	if (!clean_args)
		error_exit("malloc");
	// Second pass: Populate the clean argument list.
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "<", 2) == 0 || ft_strncmp(args[i], ">", 2) == 0)
			i += 2; // Skip operator and filename again
		else
			clean_args[j++] = ft_strdup(args[i++]);
	}
	clean_args[j] = NULL;
	return (clean_args);
}