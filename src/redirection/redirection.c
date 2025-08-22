/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 08:25:34 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/22 14:51:24 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/**
 * @brief Handles I/O redirection in the child process before command execution.
 *
 * @param input_file The file to redirect standard input from.
 * @param output_file The file to redirect standard output to.
 *
 * This function is called in the child process. It opens the specified files
 * and uses dup2 to replace STDIN_FILENO and STDOUT_FILENO.
 */
// Main redirection function
void	redirect_io(char *input_file, char *output_file, int output_mode)
{
	if (input_file)
		open_and_redirect_input(input_file);
			
	if (output_file)
		open_and_redirect_output(output_file, output_mode);
}

/* 0 ok, -1 error */
// int redirect_io(char *input_file, char *output_file, int output_mode)
// {
//     if (input_file)
// 	{
//         if (open_and_redirect_input(input_file) < 0)
//             return -1;                 /*  stop: don't touch output */
//     }
//     if (output_file)
// 	{
//         if (open_and_redirect_output(output_file, output_mode) < 0)
//             return -1;
//     }
//     return 0;
// }

// handle_redirection() Helper: Process a single redirection and update input/output file pointers
// static int	process_redirection_token(char **args, int i, t_env_var *env_list, int last_exit_status,
// 			char **final_input_file, char **final_output_file, int *output_mode, char **heredoc_file)
// {
//     if (!args[i + 1])
//     {
//         ft_putstr_fd("minishell: syntax error near `newline'\n", 2);
//         return (-1);
//     }
// 	// KEY CHANGE HERE: Validate the token after the redirection operator.
//     if (is_redirection(args[i + 1]))
//     {
//         // ft_putstr_fd("minishell: syntax error near unexpected token `", 2); // DEBUG: Print error if next token is also a redirection
//         // ft_putstr_fd(args[i + 1], 2); // DEBUG: Print the next token
//         // ft_putstr_fd("'\n", 2); // DEBUG: Print the closing quote
// 		// KEY CHANGE HERE: Use ft_error to print the error message

// 		char *error_msg_prefix = "syntax error near unexpected token `";
//         char *error_msg_suffix = "'";
//         char *full_msg = ft_strjoin(error_msg_prefix, args[i + 1]);
//         char *final_msg = ft_strjoin(full_msg, error_msg_suffix);
        
//         ft_error("", final_msg);

//         free(full_msg);
//         free(final_msg);
//         return (-1);
//     }

// 	if (ft_strncmp(args[i], "<<", 3) == 0)
// 	{
// 		if (*heredoc_file)
// 		{
// 			unlink(*heredoc_file);
// 			free(*heredoc_file);
// 			*heredoc_file = NULL; // Ensure it's set to NULL after freeing
// 		}
// 		*heredoc_file = handle_heredoc(args[i + 1], env_list, last_exit_status);
// 		if (!*heredoc_file)
// 			return (-1);
// 		free(*final_input_file);
// 		*final_input_file = ft_strdup(*heredoc_file);
// 	}
//     else if (ft_strncmp(args[i], "<", 2) == 0)
//     {
//         free(*final_input_file);
//         *final_input_file = ft_strdup(args[i + 1]);
//     }
//     else if (ft_strncmp(args[i], ">", 2) == 0)
//     {
// 		if (process_output_file(args[i + 1], 0) != 0) // 0 = truncate
//             return (-1);
//         free(*final_output_file);
//         *final_output_file = ft_strdup(args[i + 1]);
// 		*output_mode = 1; // 1 = O_TRUNC
//     }
// 	else if (ft_strncmp(args[i], ">>", 3) == 0)
// 	{
// 		if (process_output_file(args[i + 1], 1) != 0) // 1 = append
// 			return (-1);
// 		free(*final_output_file);
// 		*final_output_file = ft_strdup(args[i + 1]);
// 		*output_mode = 2; // 2 = O_APPEND
// 	}
//     return (0);
// }

static int	process_redirection_token(char **args, int i, t_env_var *env_list, int last_exit_status,
			char **final_input_file, char **final_output_file, int *output_mode, char **heredoc_file)
{
	// 1️⃣ Make sure there is a token after the redirection operator
	if (!args[i + 1])
	{
		ft_error("", "syntax error near unexpected token `newline'");
		return (-1);
	}

	// 2️⃣ Check if the next token is also a redirection
	if (is_redirection(args[i + 1]))
	{
		char *msg = ft_strjoin("syntax error near unexpected token `", args[i + 1]);
		char *full_msg = ft_strjoin(msg, "'");
		ft_error("", full_msg);
		free(msg);
		free(full_msg);
		return (-1);
	}

	// 3️⃣ Handle heredoc
	if (ft_strncmp(args[i], "<<", 3) == 0)
	{
		if (*heredoc_file)
		{
			unlink(*heredoc_file);
			free(*heredoc_file);
			*heredoc_file = NULL;
		}
		*heredoc_file = handle_heredoc(args[i + 1], env_list, last_exit_status);
		if (!*heredoc_file)
			return (-1);
		free(*final_input_file);
		*final_input_file = ft_strdup(*heredoc_file);
	}
	// 4️⃣ Handle input redirection
	else if (ft_strncmp(args[i], "<", 2) == 0)
	{
		free(*final_input_file);
		*final_input_file = ft_strdup(args[i + 1]);
		// EXIT CODE BUG FIX
		// 
		// if (access(*final_input_file, F_OK) != 0)
		// {
		// 	ft_error(args[i + 1], "No such file or directory");
		// 	return (-1);
		// }
		// if (access(*final_input_file, R_OK) != 0)
		// {
		// 	ft_error(args[i + 1], "Permission denied");
		// 	return (-1);
		// }
	}
	// 5️⃣ Handle output truncation
	else if (ft_strncmp(args[i], ">", 2) == 0)
	{
		if (process_output_file(args[i + 1], 0) != 0)
			return (-1);
		free(*final_output_file);
		*final_output_file = ft_strdup(args[i + 1]);
		*output_mode = 1; // O_TRUNC
	}
	// 6️⃣ Handle output append
	else if (ft_strncmp(args[i], ">>", 3) == 0)
	{
		if (process_output_file(args[i + 1], 1) != 0)
			return (-1);
		free(*final_output_file);
		*final_output_file = ft_strdup(args[i + 1]);
		*output_mode = 2; // O_APPEND
	}

	return (0);
}

// handle_redirection() Helper: Build clean argument array
static char	**build_clean_args(char **args, int argc)
{
    char	**clean_args = (char **)malloc(sizeof(char *) * (argc + 1));
    int		i = 0, j = 0;
    if (!clean_args)
        // error_exit("malloc"); // DEBUG: Print error if malloc fails
		ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
    while (args[i])
    {
        if (is_redirection(args[i]))
            i += 2;
        else
            clean_args[j++] = ft_strdup(args[i++]);
    }
    clean_args[j] = NULL;
    return clean_args;
}

/**
 * @brief Parses args for multiple redirections and returns a clean command array.
 * It finds the *last* input and *last* output file for actual redirection,
 * but processes *all* output files to create/truncate them.
 * @return A new, null-terminated array of strings containing only the command
 * and its arguments. Returns NULL on syntax or file error.
 */
// Main function split into helpers
char	**handle_redirection(char **args, char **final_input_file, char **final_output_file,
							int *output_mode, char **heredoc_file, t_env_var *env_list, int last_exit_status)
{
    int		i = 0;
    int		argc;

    *final_input_file = NULL;
    *final_output_file = NULL;
	*output_mode = 0; // Initialize output_mode
	*heredoc_file = NULL;
    // First pass: process redirections and count clean args
    argc = 0;
    while (args[i])
    {
        if (is_redirection(args[i]))
        {
            if (process_redirection_token(args, i, env_list, last_exit_status, final_input_file, final_output_file,
											output_mode, heredoc_file) != 0)
            {
				if (*heredoc_file)
				{
					unlink(*heredoc_file);
					free(*heredoc_file);
					*heredoc_file = NULL; // Ensure it's set to NULL after freeing
				}
				// Free any filenames that might have been allocated before the error
				free(*final_input_file);
				free(*final_output_file);
				*final_input_file = NULL;
				*final_output_file = NULL;
				return (NULL);
			}
            i += 2;
        }
        else
        {
            argc++;
            i++;
        }
    }
    // Second pass: build clean argument array
    return build_clean_args(args, argc);
}