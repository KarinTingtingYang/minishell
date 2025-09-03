/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/04 10:15:34 by tiyang        #+#    #+#                 */
/*   Updated: 2025/09/03 13:27:02 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int process_heredoc_input(char **line, int fd, int expand_content,
                               t_expand_data *data, char *actual_delimiter,
                               char *tmp_filename)
{
    *line = readline("> ");
    if (g_signal_received == SIGINT)
    {
        cleanup_heredoc(*line, fd, tmp_filename, actual_delimiter);
        return (0);
    }
    if (!*line)
    {
        handle_eof_warning(actual_delimiter);
        return (1);
    }
    if (ft_strncmp(*line, actual_delimiter, ft_strlen(actual_delimiter) + 1) == 0)
    {
        free(*line);
        *line = NULL;
        return (1);
    }
    return (write_heredoc_line(*line, fd, expand_content, data));
}

char *handle_heredoc(const char *delimiter, t_env_var *env_list, int last_exit_status)
{
    char            *line;
    char            *tmp_filename;
    char            *actual_delimiter;
    int             fd;
    int             expand_content;
    t_expand_data   expand_data;

    expand_data.env_list = env_list;
    expand_data.last_exit_status = last_exit_status;
    expand_content = !(ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '"'));

    if (!setup_heredoc(delimiter, &actual_delimiter, &tmp_filename, &fd))
        return (NULL);

    rl_event_hook = signal_event_hook;
    while (1)
    {
        if (!process_heredoc_input(&line, fd, expand_content, &expand_data,
                                 actual_delimiter, tmp_filename))
            return (NULL);
        if (!line)
            break;
    }
    
    close(fd);
    free(actual_delimiter);
    rl_event_hook = NULL;
    return (tmp_filename);
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


