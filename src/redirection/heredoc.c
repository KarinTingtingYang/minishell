/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 10:15:34 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/05 08:58:17 by tiyang        ########   odam.nl         */
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
 * @brief The event hook for heredoc. Readline calls this function periodically.
 * @return 0 on success.
 */
static int	heredoc_event_hook(void)
{
    // If our signal handler has set the global variable...
	if (g_signal_received == SIGINT)
	{
        // ...tell readline to stop waiting for input and return immediately.
		rl_done = 1;
	}
	return (0);
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
char *handle_heredoc(const char *delimiter)
{
	char	*line;
	char	*tmp_filename;
	int		fd;

	tmp_filename = generate_unique_heredoc_file(&fd);
	if (!tmp_filename)
	{
		perror("minishell: heredoc");
		return (NULL);
	}
	 // Set the readline event hook. This function will be called
    // periodically by readline while it waits for input.
	rl_event_hook = heredoc_event_hook;
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
            ft_putstr_fd((char *)delimiter, 2);
            ft_putstr_fd("')\n", 2);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	rl_event_hook = NULL; // Unset the hook before returning
	return (tmp_filename);
}


