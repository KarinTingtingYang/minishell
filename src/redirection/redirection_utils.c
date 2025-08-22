/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:10:49 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/22 14:13:59 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Processes an output file by creating/truncating it.
 * This mimics shell behavior where output files are processed during parsing.
 * It opens the file to ensure it's accessible and to truncate it, then
 * immediately closes it without changing the shell's standard output.
 */
// APPEND REDIRECTION: ADDED FLAG PARAMETER IS_APPEND (0=truncate, 1=append)
int	process_output_file(char *output_file, int is_append)
{
	int	fd_out;

	// Open the file with flags to create it if it doesn't exist
	// and truncate it if it does.
	if (is_append)
		fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
	{
		// Use perror to print a descriptive error message to STDERR
		// perror(output_file); // DEBUG: Print error if open fails
		ft_error_and_exit(output_file, strerror(errno), EXIT_FAILURE);
		// Return -1 to signal an error to the caller
		return (-1);
	}
	// Immediately close the file descriptor. We're done with it for now.
	close(fd_out);
	return (0);
}


// Helper: Open and redirect input
void	open_and_redirect_input(char *input_file)
{
    int	fd_in;

    fd_in = open(input_file, O_RDONLY);
    if (fd_in == -1)
    {
        // perror(input_file); // DEBUG: Print error if open fails
		ft_error_and_exit(input_file, strerror(errno), EXIT_FAILURE);
        // exit(EXIT_FAILURE);
    }
    if (dup2(fd_in, STDIN_FILENO) == -1)
    {
        // perror("dup2"); // DEBUG: Print error if dup2 fails
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
        // exit(EXIT_FAILURE);
    }
    close(fd_in);
}

// Helper: Open and redirect output
// APPEND REDIRECTION: ADDED FLAG PARAMETER 0=none, 1=truncate(>), 2=append(>>)
void	open_and_redirect_output(char *output_file, int output_mode)
{
	int	fd_out;
	int	flags;

	if (output_mode == 2) // 2 == O_APPEND
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else // Default to O_TRUNC
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd_out = open(output_file, flags, 0644);
	if (fd_out == -1)
	{
		// perror(output_file); // DEBUG: Print error if open fails
		ft_error_and_exit(output_file, strerror(errno), EXIT_FAILURE);
		// exit(EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		// perror("dup2"); // DEBUG: Print error if dup2 fails
		ft_error_and_exit("dup2", strerror(errno), EXIT_FAILURE);
		// exit(EXIT_FAILURE);
	}
	close(fd_out);
}


// /* 0 ok, -1 error (errno is set) */
// int open_and_redirect_input(const char *input_file)
// {
//     int fd = open(input_file, O_RDONLY);
//     if (fd < 0)
//         return -1;
//     if (dup2(fd, STDIN_FILENO) < 0)
// 	{
//         close(fd);
//         return -1;
//     }
//     close(fd);
//     return 0;
// }

// /* output_mode: 2 = append (>>), else truncate (>) */
// int open_and_redirect_output(const char *output_file, int output_mode)
// {
//     int flags = O_WRONLY | O_CREAT | (output_mode == 2 ? O_APPEND : O_TRUNC);
//     int fd = open(output_file, flags, 0644);
//     if (fd < 0)
//         return -1;
//     if (dup2(fd, STDOUT_FILENO) < 0)
// 	{
//         close(fd);
//         return -1;
//     }
//     close(fd);
//     return 0;
// }

// Helper: Count non-redirection arguments
int	count_clean_args(char **args)
{
    int i = 0, count = 0;
    while (args[i])
    {
        if (is_redirection(args[i]))
            i += 2;
        else
        {
            count++;
            i++;
        }
    }
    return count;
}

// Helper: Check if token is a redirection operator
int	is_redirection(const char *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token, "<", 2) == 0)
		return (1);
	if (ft_strncmp(token, ">", 2) == 0)
		return (1);
	if (ft_strncmp(token, ">>", 3) == 0)
		return (1);
	if (ft_strncmp(token, "<<", 3) == 0)
		return (1);
	return (0);
}