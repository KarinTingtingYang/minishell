/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_helper.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/09/03 12:16:13 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 13:27:45 by tiyang        ########   odam.nl         */
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

void handle_eof_warning(const char *actual_delimiter)
{
    ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
    ft_putstr_fd((char *)actual_delimiter, 2);
    ft_putstr_fd("')\n", 2);
}

char *process_variable(char *line, size_t *i, char *result, t_expand_data *data)
{
    char next_char = line[*i + 1];
    if (ft_isalnum(next_char) || next_char == '_' || next_char == '?')
        *i = handle_variable_expansion(line, *i, &result, data);
    else
    {
        result = append_char(result, line[*i]);
        (*i)++;
    }
    return result;
}

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
