/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 11:10:49 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/26 10:31:02 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Processes an output file by creating/truncating it.
 * This mimics shell behavior where all output files in a command are created.
 */
// Helper function to handle the creation/truncation of ALL
// specified output files, not just the last one.
int	process_output_file(char *output_file)
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

// Helper: Open and redirect input
void	open_and_redirect_input(char *input_file)
{
    int	fd_in;

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

// Helper: Open and redirect output
void	open_and_redirect_output(char *output_file)
{
    int	fd_out;

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
    return (ft_strncmp(token, "<", 2) == 0 || ft_strncmp(token, ">", 2) == 0);
}
