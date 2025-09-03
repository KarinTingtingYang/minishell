/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 12:16:13 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 15:22:49 by tiyang        ########   odam.nl         */
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
char *generate_unique_heredoc_file(int *out_fd)
{
	char	*base;
	char	*num_str;
	char	*filename;
	int		i;

	base = "/tmp/minishell-heredoc-";
	i = 0;
	while (i < 1024)
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
			return (filename);
		free(filename);
		if (errno != EEXIST)
			return (NULL);
		i++;
	}
	return (NULL);
}

/**
 * @brief Cleans up resources used during heredoc processing.
 *
 * This function frees the provided line, closes the file descriptor,
 * unlinks (deletes) the temporary heredoc file, and frees the associated
 * filename and delimiter strings.
 *
 * @param line The line buffer to free.
 * @param fd The file descriptor to close.
 * @param tmp_filename The temporary filename to unlink and free.
 * @param actual_delimiter The actual delimiter string to free.
 */
void cleanup_heredoc(char *line, int fd, char *tmp_filename, 
                          char *actual_delimiter)
{
    free(line);
    close(fd);
    unlink(tmp_filename);
    free(tmp_filename);
    free(actual_delimiter);
    rl_event_hook = NULL;
}

/**
 * @brief Handles end-of-file (EOF) warning for heredoc.
 *
 * This function prints a warning message to stderr indicating that
 * the heredoc was terminated by EOF instead of the expected delimiter.
 *
 * @param actual_delimiter The expected delimiter string.
 */
void handle_eof_warning(const char *actual_delimiter)
{
    ft_putstr_fd("minishell: warning: ", 2);
	ft_putstr_fd("here-document delimited by end-of-file (wanted `", 2);
    ft_putstr_fd((char *)actual_delimiter, 2);
    ft_putstr_fd("')\n", 2);
}

/**
 * @brief Writes a line to the heredoc file, expanding variables if needed.
 *
 * This function expands the line if `expand_content` is true, then writes
 * the (possibly expanded) line followed by a newline to the provided file
 * descriptor.
 *
 * @param line The line to write.
 * @param fd The file descriptor of the heredoc file.
 * @param expand_content Whether to expand variables in the line.
 * @param data The expansion data containing environment variables and last exit status.
 * @return 1 on success, 0 on failure.
 */
char *process_variable(char *line, size_t *i, char *result, t_expand_data *data)
{
    char next_char;

	next_char = line[*i + 1];
    if (ft_isalnum(next_char) || next_char == '_' || next_char == '?')
        *i = handle_variable_expansion(line, *i, &result, data);
    else
    {
        result = append_char(result, line[*i]);
        (*i)++;
    }
    return result;
}

/**
 * @brief Processes a single line of heredoc input.
 *
 * This function checks for signal interrupts, EOF, and the delimiter.
 * If the line is valid, it writes it to the heredoc file, expanding
 * variables if necessary.
 *
 * @param line A pointer to the line buffer. This may be set to NULL
 * if the delimiter is reached or EOF occurs.
 * @param ctx The heredoc context containing file descriptor, expansion
 * settings, and other relevant data.
 * @return 1 if processing should continue, 0 on error or if the line
 * was NULL (indicating end of input).
 */
char *expand_heredoc_line(char *line, t_expand_data *data)
{
    char    *result;
    size_t  i;

    result = ft_strdup("");
    if (!result)
        return (NULL);
    i = 0;
    while (line[i])
    {
        if (line[i] == '$')
            result = process_variable(line, &i, result, data);
        else
        {
            result = append_char(result, line[i]);
            i++;
        }
    }
    return (result);
}
