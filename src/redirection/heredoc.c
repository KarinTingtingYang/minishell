/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 10:15:34 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/04 09:44:07 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Event hook for readline to handle signals during heredoc input.
 *
 * This function is called periodically by readline while waiting
 * for user input. It checks if a signal has been received
 * (e.g., SIGINT) and returns 1 to interrupt the input process if so.
 * Otherwise, it returns 0 to continue waiting.
 *
 * @return 1 if a signal was received, 0 otherwise.
 */
static int	write_heredoc_line(char *line, int fd, int expand_content,
									t_expand_data *data)
{
	char	*expanded_line;

	if (expand_content)
	{
		expanded_line = expand_heredoc_line(line, data);
		ft_putstr_fd(expanded_line, fd);
		free(expanded_line);
	}
	else
		ft_putstr_fd(line, fd);
	ft_putstr_fd("\n", fd);
	free(line);
	return (1);
}

/**
 * @brief Sets up the heredoc by generating a unique filename
 *        and processing the delimiter.
 *
 * This function removes quotes from the delimiter if present, generates a unique
 * temporary filename for the heredoc, and opens the file for writing. It handles
 * errors appropriately and returns 1 on success or 0 on failure.
 *
 * @param delimiter The original delimiter string (may contain quotes).
 * @param actual_delimiter Pointer to store the processed delimiter
 *                         without quotes.
 * @param tmp_filename Pointer to store the generated temporary filename.
 * @param fd Pointer to store the file descriptor of the opened temporary file.
 * @return 1 on success, 0 on failure.
 */
static int	setup_heredoc(const char *delimiter, char **actual_delimiter,
								char **tmp_filename, int *fd)
{
	*actual_delimiter = remove_quotes_and_join((char *)delimiter);
	if (*actual_delimiter == NULL)
		return (0);
	*tmp_filename = generate_unique_heredoc_file(fd);
	if (*tmp_filename == NULL)
	{
		free(*actual_delimiter);
		ft_error_and_exit("heredoc", strerror(errno), EXIT_FAILURE);
		return (0);
	}
	return (1);
}

/**
 * @brief Processes a single line of heredoc input.
 *
 * This function reads a line from the user, checks for signals, compares
 * it to the delimiter, and writes it to the heredoc file if appropriate.
 *
 * @param line Pointer to store the read line.
 * @param fd The file descriptor of the heredoc file.
 * @param expand_content Flag indicating if to expand variables in the line.
 * @param data Pointer to the expansion data structure.
 * @param actual_delimiter The processed delimiter string.
 * @param tmp_filename The temporary filename for cleanup on interrupt.
 * @return 1 to continue, 0 on interrupt or error, or 1 if delimiter is reached.
 */
static int	process_heredoc_input(char **line, t_heredoc_context *ctx)
{
	*line = readline("> ");
	if (g_signal_received == SIGINT)
	{
		cleanup_heredoc(*line, ctx->fd, ctx->tmp_filename,
			ctx->actual_delimiter);
		return (0);
	}
	if (*line == NULL)
	{
		handle_eof_warning(ctx->actual_delimiter);
		return (1);
	}
	if (ft_strncmp(*line, ctx->actual_delimiter,
			ft_strlen(ctx->actual_delimiter) + 1) == 0)
	{
		free(*line);
		*line = NULL;
		return (1);
	}
	return (write_heredoc_line(*line, ctx->fd, ctx->expand_content, ctx->data));
}

/**
 * @brief Handles heredoc (<<) input by reading from stdin.
 *
 * Reads input from the user line by line until the specified delimiter
 * is entered. The input is written to a unique temporary file.
 *
 * @param delimiter The string that signals the end of input.
 * @param env_list The linked list of environment variables for expansion.
 * @param last_exit_status The last exit status for $? expansion.
 * @return The path to the temporary file, or NULL on failure.
 */
char	*handle_heredoc(const char *delimiter, t_env_var *env_list,
	int last_exit_status)
{
	char				*line;
	t_heredoc_context	ctx;
	t_expand_data		expand_data;

	expand_data.env_list = env_list;
	expand_data.last_exit_status = last_exit_status;
	ctx.expand_content = !(ft_strchr(delimiter, '\'')
			|| ft_strchr(delimiter, '"'));
	ctx.data = &expand_data;
	if (!setup_heredoc(delimiter, &ctx.actual_delimiter,
			&ctx.tmp_filename, &ctx.fd))
		return (NULL);
	rl_event_hook = signal_event_hook;
	while (1)
	{
		if (!process_heredoc_input(&line, &ctx))
			return (NULL);
		if (line == NULL)
			break ;
	}
	close(ctx.fd);
	free(ctx.actual_delimiter);
	rl_event_hook = NULL;
	return (ctx.tmp_filename);
}

/**
 * @brief Checks if the number of here-documents in the command
 *        line exceeds the maximum allowed.
 * 
 * This function counts the number of here-document redirections
 * (<<) in the provided command line and compares it against
 * a predefined maximum limit (MAX_HEREDOCS). If the limit is exceeded,
 * it prints an error message and exits the program.
 * @param line The command line string to check.
 * @return Returns 1 if within limit, 0 if exceeded (after exiting).
 */
int	check_heredoc_limit(char *line)
{
	if (count_heredocs(line) > MAX_HEREDOCS)
	{
		ft_error_and_exit("", "maximum here-document count exceeded", 2);
		return (0);
	}
	return (1);
}
