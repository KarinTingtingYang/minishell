/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 10:15:34 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/19 08:38:47 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Generates a unique temporary filename for a heredoc.
 *
 * This function attempts to create a unique file in /tmp/ by appending
 * an incrementing number to a base name. It uses the O_CREAT and O_EXCL
 * flags with open() to ensure the file creation is atomic, preventing
 * race conditions.
 *
 * @param out_fd A pointer to an integer where the file descriptor of the
 * newly created file will be stored.
 * @return A dynamically allocated string containing the unique filename,
 * or NULL on failure.
 */
static char *generate_unique_heredoc_file(int *out_fd)
{
	char	*base;
	char	*num_str;
	char	*filename;
	int		i;

	base = "/tmp/minishell-heredoc-";
	i = 0;
	while (i < 1024) // Try up to 1024 times to find a unique name
	{
		num_str = ft_itoa(i);
		if (!num_str)
			return (NULL);
		filename = ft_strjoin(base, num_str);
		free(num_str);
		if (!filename)
			return (NULL);
		*out_fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (*out_fd != -1)
			return (filename); // Success!
		free(filename);
		if (errno != EEXIST)
			return (NULL); // A different error occurred
		i++;
	}
	return (NULL);
}

/**
 * @brief Handles heredoc (<<) input by reading from stdin.
 *
 * Reads input from the user line by line until the specified delimiter
 * is entered. The input is written to a unique temporary file.
 *
 * @param delimiter The string that signals the end of input.
 * @return The path to the temporary file, or NULL on failure.
 */
char *handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status)
{
	char	*line;
	char	*tmp_filename;
	int		fd;
	// FIX: Add logic for expansion and quoted delimiters
	int		expand_content;
	char	*actual_delimiter;

	expand_content = 1;
	// Check if the delimiter was quoted. If so, don't expand content.
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
		expand_content = 0;
	// Remove quotes from the delimiter itself
	actual_delimiter = remove_quotes_and_join((char *)delimiter);
	if (!actual_delimiter)
		return (NULL); // Or handle error appropriately
	printf("actual delimiter is [%s]\n", actual_delimiter);
	tmp_filename = generate_unique_heredoc_file(&fd);
	if (!tmp_filename)
	{
		free(actual_delimiter);
		// perror("minishell: heredoc"); // DEBUG: Print error if file creation fails
		ft_error_and_exit("heredoc", strerror(errno), EXIT_FAILURE);
		// return (NULL);
	}
	 // Set the readline event hook. This function will be called
    // periodically by readline while it waits for input.
	rl_event_hook = signal_event_hook;
	while (1)
	{
		line = readline("> ");
		// After readline returns, check if it was due to our signal.
		if (g_signal_received == SIGINT)
		{
			free(line);
			close(fd);
			unlink(tmp_filename);
			free(tmp_filename);
			free(actual_delimiter);
            rl_event_hook = NULL; // Unset the hook
			return (NULL); // Abort the command
		}
		// if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		// {
		// 	free(line);
		// 	break ;
		// }
		if (!line) // Ctrl+D pressed
		{
			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
            ft_putstr_fd((char *)actual_delimiter, 2);
            ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strncmp(line, actual_delimiter, ft_strlen(actual_delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		//ft_putstr_fd(line, fd);
		// FIX: Expand the line if needed before writing to the file
		if (expand_content)
		{
			char *expanded_line = expand_variables(line, env_list, last_exit_status, NO_QUOTE);
			ft_putstr_fd(expanded_line, fd);
			free(expanded_line);
		}
		else
		{
			ft_putstr_fd(line, fd);
		}
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	free(actual_delimiter); // Free the unquoted delimiter
	rl_event_hook = NULL; // Unset the hook before returning
	return (tmp_filename);
}


