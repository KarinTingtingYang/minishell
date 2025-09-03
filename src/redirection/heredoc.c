/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 10:15:34 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 15:18:53 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * @brief Event hook for readline to handle signals during heredoc input.
 *
 * This function is called periodically by readline while waiting for user input.
 * It checks if a signal has been received (e.g., SIGINT) and returns 1 to
 * interrupt the input process if so. Otherwise, it returns 0 to continue waiting.
 *
 * @return 1 if a signal was received, 0 otherwise.
 */
static int write_heredoc_line(char *line, int fd, int expand_content, 
                            t_expand_data *data)
{
    char *expanded_line;

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
 * @brief Sets up the heredoc by generating a unique filename and processing the delimiter.
 *
 * This function removes quotes from the delimiter if present, generates a unique
 * temporary filename for the heredoc, and opens the file for writing. It handles
 * errors appropriately and returns 1 on success or 0 on failure.
 *
 * @param delimiter The original delimiter string (may contain quotes).
 * @param actual_delimiter Pointer to store the processed delimiter without quotes.
 * @param tmp_filename Pointer to store the generated temporary filename.
 * @param fd Pointer to store the file descriptor of the opened temporary file.
 * @return 1 on success, 0 on failure.
 */
static int setup_heredoc(const char *delimiter, char **actual_delimiter, 
                        char **tmp_filename, int *fd)
{
    *actual_delimiter = remove_quotes_and_join((char *)delimiter);
    if (!*actual_delimiter)
        return (0);
    *tmp_filename = generate_unique_heredoc_file(fd);
    if (!*tmp_filename)
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
static int process_heredoc_input(char **line, t_heredoc_context *ctx)
{
    *line = readline("> ");
    if (g_signal_received == SIGINT)
    {
        cleanup_heredoc(*line, ctx->fd, ctx->tmp_filename,
			ctx->actual_delimiter);
        return (0);
    }
    if (!*line)
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
char *handle_heredoc(const char *delimiter, t_env_var *env_list,
	int last_exit_status)
{
    char				*line;
	t_heredoc_context	ctx;
    t_expand_data		expand_data;

    expand_data.env_list = env_list;
    expand_data.last_exit_status = last_exit_status;
	ctx.expand_content = !(ft_strchr(delimiter, '\'') ||
							ft_strchr(delimiter, '"'));
	ctx.data = &expand_data;
    if (!setup_heredoc(delimiter, &ctx.actual_delimiter,
		&ctx.tmp_filename, &ctx.fd))
        return (NULL);
    rl_event_hook = signal_event_hook;
    while (1)
    {
        if (!process_heredoc_input(&line, &ctx))
            return (NULL);
        if (!line)
            break;
    }
    close(ctx.fd);
    free(ctx.actual_delimiter);
    rl_event_hook = NULL;
    return (ctx.tmp_filename);
}

/**
 * @brief Checks if the number of here-documents in the command line exceeds the maximum allowed.
 * 
 * This function counts the number of here-document redirections (<<) in the provided
 * command line and compares it against a predefined maximum limit (MAX_HEREDOCS).
 * If the limit is exceeded, it prints an error message and exits the program.
 * @param line The command line string to check.
 * @return Returns 1 if within limit, 0 if exceeded (after exiting).
 */
int check_heredoc_limit(char *line)
{
    if (count_heredocs(line) > MAX_HEREDOCS)
    {
        ft_error_and_exit("", "maximum here-document count exceeded", 2);
        return (0);
    }
    return (1);
}

// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------

// /**
//  * @brief Expands variables in a line of heredoc input.
//  *
//  * This is a special version of the expander that ignores quotes and expands
//  * any valid variable it finds. This matches bash's heredoc behavior when
//  * the delimiter is not quoted.
//  */
// static char	*expand_heredoc_line(char *line, t_expand_data *data)
// {
// 	char	*result;
// 	size_t	i;

// 	result = ft_strdup("");
// 	if (!result)
// 		return (NULL);
// 	i = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '$')
// 		{
// 			char next_char = line[i + 1];
// 			if (ft_isalnum(next_char) || next_char == '_' || next_char == '?')
// 				i = handle_variable_expansion(line, i, &result, data);
// 			else
// 			{
// 				result = append_char(result, line[i]);
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			result = append_char(result, line[i]);
// 			i++;
// 		}
// 	}
// 	return (result);
// }
// /**
//  * @brief Handles heredoc (<<) input by reading from stdin.
//  *
//  * Reads input from the user line by line until the specified delimiter
//  * is entered. The input is written to a unique temporary file.
//  *
//  * @param delimiter The string that signals the end of input.
//  * @return The path to the temporary file, or NULL on failure.
//  */
// char *handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status)
// {
// 	char	*line;
// 	char	*tmp_filename;
// 	int		fd;
// 	// FIX: Add logic for expansion and quoted delimiters
// 	int		expand_content;
// 	char	*actual_delimiter;
// 	t_expand_data	expand_data; // Create a data struct to pass to the helper

// 	expand_data.env_list = env_list;
// 	expand_data.last_exit_status = last_exit_status;
	
// 	expand_content = 1;
// 	// Check if the delimiter was quoted. If so, don't expand content.
// 	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'))
// 		expand_content = 0;
// 	// Remove quotes from the delimiter itself
// 	actual_delimiter = remove_quotes_and_join((char *)delimiter);
// 	if (!actual_delimiter)
// 		return (NULL); // Or handle error appropriately
// 	printf("actual delimiter is [%s]\n", actual_delimiter);
// 	tmp_filename = generate_unique_heredoc_file(&fd);
// 	if (!tmp_filename)
// 	{
// 		free(actual_delimiter);
// 		// perror("minishell: heredoc"); // DEBUG: Print error if file creation fails
// 		ft_error_and_exit("heredoc", strerror(errno), EXIT_FAILURE);
// 		// return (NULL);
// 	}
// 	 // Set the readline event hook. This function will be called
//     // periodically by readline while it waits for input.
// 	rl_event_hook = signal_event_hook;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		// After readline returns, check if it was due to our signal.
// 		if (g_signal_received == SIGINT)
// 		{
// 			free(line);
// 			close(fd);
// 			unlink(tmp_filename);
// 			free(tmp_filename);
// 			free(actual_delimiter);
//             rl_event_hook = NULL; // Unset the hook
// 			return (NULL); // Abort the command
// 		}
// 		// if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
// 		// {
// 		// 	free(line);
// 		// 	break ;
// 		// }
// 		if (!line) // Ctrl+D pressed
// 		{
// 			ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
//             ft_putstr_fd((char *)actual_delimiter, 2);
//             ft_putstr_fd("')\n", 2);
// 			break ;
// 		}
// 		if (ft_strncmp(line, actual_delimiter, ft_strlen(actual_delimiter) + 1) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		//ft_putstr_fd(line, fd);
// 		// FIX: Expand the line if needed before writing to the file
// 		if (expand_content)
// 		{
// 			char *expanded_line = expand_heredoc_line(line, &expand_data);
// 			ft_putstr_fd(expanded_line, fd);
// 			free(expanded_line);
// 		}
// 		else
// 		{
// 			ft_putstr_fd(line, fd);
// 		}
// 		ft_putstr_fd("\n", fd);
// 		free(line);
// 	}
// 	close(fd);
// 	free(actual_delimiter); // Free the unquoted delimiter
// 	rl_event_hook = NULL; // Unset the hook before returning
// 	return (tmp_filename);
// }


