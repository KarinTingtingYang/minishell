/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/30 14:14:42 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern char **environ;
//extern volatile sig_atomic_t g_child_running; // Declare extern for global flag

// static void handle_execve_error(char *cmd_path, char **args, char **path_dirs)
// {
// 	(void)args;
// 	(void)path_dirs;

// 	if (errno == ENOENT)
// 	{
//     	ft_error_and_exit(cmd_path, "command not found", 127);
// 	}
// 	else
// 	{
// 		ft_error_and_exit(cmd_path, strerror(errno), EXIT_FAILURE);
// 	}
// 	free(cmd_path);
// }

// static void handle_execve_error(char *cmd_path, char **args, char **path_dirs)
// {
//     (void)path_dirs;

//     if (!cmd_path || !args || !args[0])
//     {
//         // Safety fallback
//         ft_error_and_exit("minishell", "Unknown execution error", 126);
//     }

//     if (errno == ENOENT)
//         ft_error_and_exit(args[0], "No such file or directory", 127);
//     else if (errno == EACCES)
//         ft_error_and_exit(args[0], "Permission denied", 126);
//     else if (errno == ENOEXEC)
//         ft_error_and_exit(args[0], "Exec format error", 126);
//     else
//         ft_error_and_exit(args[0], strerror(errno), 126);

//     free(cmd_path); // cmd_path is now properly used
// }


// void execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list)
// {
// 	char **envp;

//     reset_child_signal_handlers();
//     envp = env_list_to_array(env_list);
//     if (cmd_path == NULL)
//     {
//         if (args && args[0])
// 			ft_error_and_exit(args[0], "command not found", 127);
//         if (envp)
//             free_split(envp);
//         exit(127);
//     }
//     execve(cmd_path, args, envp);
//     free_split(envp);
//     handle_execve_error(cmd_path, args, path_dirs);
// }

// void execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list)
// {
//     char        **envp;
//     struct stat   st;

//     (void)path_dirs;

//     reset_child_signal_handlers();
//     envp = env_list_to_array(env_list);

//     /* Resolver failed: choose correct message based on args[0] */
//     if (cmd_path == NULL)
//     {
//         if (args && args[0])
//         {
//             if (ft_strchr(args[0], '/'))
//             {
//                 if (envp) free_split(envp);
//                 ft_error_and_exit(args[0], "No such file or directory", 127);
//             }
//             else
//             {
//                 if (envp) free_split(envp);
//                 // ft_error_and_exit(args[0], "command not found", 127);
// 				if (path_dirs == NULL)
//                 ft_error_and_exit(args[0], "No such file or directory", 127);
//             else
//                 ft_error_and_exit(args[0], "command not found", 127);
//             }
//         }
//         if (envp) free_split(envp);
//         exit(127);
//     }

//     /* Pre-checks to match bash */
//     if (stat(cmd_path, &st) == 0 && S_ISDIR(st.st_mode))
//     {
//         if (envp) free_split(envp);
//         ft_error_and_exit(args[0], "Is a directory", 126);
//     }
//     if (access(cmd_path, X_OK) != 0)
//     {
//         if (envp) free_split(envp);
//         ft_error_and_exit(args[0], "Permission denied", 126);
//     }

//     /* Try to exec */
//     execve(cmd_path, args, envp);

//     /* If we’re here, execve failed → map errno (ENOEXEC, etc.) */
//     free_split(envp);
//     handle_execve_error(cmd_path, args, NULL);
// }

/* ---- executor ---- */
void execute_cmd(char *cmd_path, char **args, char **path_dirs, t_env_var *env_list)
{
    char        **envp;
    struct stat   st;
    int           e;

    (void)path_dirs;
    reset_child_signal_handlers();
    envp = env_list_to_array(env_list);

    /* resolver failed: choose message by how argv[0] looks */
    if (cmd_path == NULL)
    {
        if (args && args[0] && ft_strchr(args[0], '/'))
        {
            if (envp) free_split(envp);
            ft_error_and_exit(args[0], "No such file or directory", 127);
        }
        if (envp) free_split(envp);
        ft_error_and_exit(args && args[0] ? args[0] : "minishell",
                          "command not found", 127);
    }

    /* precise pre-checks like bash */
    if (stat(cmd_path, &st) == -1)
    {
        e = errno;
        if (envp) free_split(envp);
        if (e == ENOTDIR)
            ft_error_and_exit(args[0], "Not a directory", 126);
        if (e == ENOENT)
            ft_error_and_exit(args[0], "No such file or directory", 127);
        ft_error_and_exit(args[0], strerror(e), 126);
    }
    if (S_ISDIR(st.st_mode))
    {
        if (envp) free_split(envp);
        ft_error_and_exit(args[0], "Is a directory", 126);
    }
    if (access(cmd_path, X_OK) == -1)
    {
        e = errno;
        if (envp) free_split(envp);
        if (e == EACCES)
            ft_error_and_exit(args[0], "Permission denied", 126);
        if (e == ENOTDIR)
            ft_error_and_exit(args[0], "Not a directory", 126);
        ft_error_and_exit(args[0], strerror(e), 126);
    }

    /* run */
    execve(cmd_path, args, envp);

    /* execve failed → map remaining cases */
    e = errno;
    free_split(envp);
    if (e == ENOEXEC)
        ft_error_and_exit(args[0], "Exec format error", 126);
    if (e == ENOTDIR)
        ft_error_and_exit(args[0], "Not a directory", 126);
    if (e == ENOENT)
        ft_error_and_exit(args[0], "No such file or directory", 127);
    if (e == EACCES)
        ft_error_and_exit(args[0], "Permission denied", 126);
    ft_error_and_exit(args[0], strerror(e), 126);
}

static int execute_single_command(char **args, t_env_var *env_list, t_process_data *process_data)
{
    t_execute_data  data;
    int             original_stdin;
    int             original_stdout;
    int             exit_status;

    ft_bzero(&data, sizeof(t_execute_data));
	// ---> THIS IS THE FIX <---
	// We must populate the data struct with the environment list.
	data.env_list = env_list;

	 // Set in_pipeline to 0 for single commands
    process_data->in_pipeline = 0;
	
    data.clean_args = handle_redirection(args, process_data, &data.input_file, 
		&data.output_file, &data.output_mode, &data.heredoc_file);
	// FIX FOR CASE 1: Syntax error in redirection (e.g., "echo >")
    if (!data.clean_args)
    {
        // ** THE FIX IS HERE **
        // First, check if the failure was due to a signal (e.g., Ctrl+C in heredoc).
        if (g_signal_received == SIGINT)
        {
            process_data->last_exit_status = 130;
            return (130);
        }
        
        // // If not a signal, handle it as a syntax or file error.
        //if (args[1] == NULL && is_redirection(args[0]))
		int last_arg_index = 0;
		while(args[last_arg_index + 1] != NULL) 
			last_arg_index++;
		if (is_redirection(args[last_arg_index]))
        {
             process_data->last_exit_status = 2; // Syntax error like ">"
             return (2);
        }
        // If not a signal, handle it as a syntax or file error.        
		process_data->last_exit_status = 1; // File error like "> /noperm"
        return (1);
    }
    
    // Fix: Handle commands that are only redirections
	 // FIX FOR CASE 2: Redirection-only command (e.g., "< nofile" or "> newfile")
    if (!data.clean_args[0])
    {
		 exit_status = 0;
        // Check input file for existence and permissions
        if (data.input_file)
        {
            int fd = open(data.input_file, O_RDONLY);
            if (fd == -1) {
                ft_error(data.input_file, strerror(errno));
                exit_status = 1;
            } else {
                close(fd);
            }
        }
        // Output files are already created/checked in handle_redirection.
        // If an output file failed, data.clean_args would be NULL and we wouldn't be here.
        
        process_data->last_exit_status = exit_status;
        free_execute_data(&data);
        return (0);
    }

    if (is_builtin(data.clean_args[0]))
    {
        original_stdin = dup(STDIN_FILENO);
        original_stdout = dup(STDOUT_FILENO);

        if (apply_builtin_redirection(data.input_file, data.output_file, data.output_mode) == -1)
            exit_status = 1;
        else
            exit_status = run_builtin(data.clean_args, process_data);

        dup2(original_stdin, STDIN_FILENO);
        dup2(original_stdout, STDOUT_FILENO);
        close(original_stdin);
        close(original_stdout);
    }
    else // Not a builtin
    {
        char **path_dirs = find_path_dirs(env_list);
        
        // Find command path, handle errors if not found
        data.cmd_path = find_full_cmd_path(data.clean_args[0], path_dirs);
        free_split(path_dirs);

        // if (!data.cmd_path)
        // {
        //     // ft_error(data.clean_args[0], "command not found");
        //     exit_status = 127;
        // }
        // else
        // {
        exit_status = execute_prepared_command(&data, process_data);
        // }
    }

    process_data->last_exit_status = exit_status;
    free_execute_data(&data);
    return (exit_status);
}

static int prepare_and_run_pipeline(char *line,  t_env_var *env_list, t_process_data *process_data)
{
    char **parts;
	t_command **cmds;
	char **path_dirs;
	int status;
    int count = 0;

	parts = NULL;
	count = 0;	
	// if (!precheck_pipe_syntax(line))
	// {
	// 	process_data->last_exit_status = 2;
	// 	return (2);
	// }		
    cmds = prepare_pipeline_commands(line, &count, &parts, process_data);
    if (cmds == NULL)
	{
		// If command preparation failed, check if it was due to a signal.
		if (g_signal_received == SIGINT)
		{
			process_data->last_exit_status = 130;
			return (130);
		}
		process_data->last_exit_status = 2;
		return (2);
	}
    path_dirs = find_path_dirs(env_list);
	// if (!path_dirs)
	// {
	// 	ft_putendl_fd("Error: PATH variable not found", 2);
	// 	return (1);
	// }
    status = run_command_pipeline(cmds, count, path_dirs, env_list);
    // free_split(path_dirs);
	if (path_dirs)
    	free_split(path_dirs);
    free_commands_recursive(cmds, 0, count);
    free(cmds);
    free_split(parts);
	process_data->last_exit_status = status;
    return (status);
}

/**
 * @brief Checks for the presence of an unquoted pipe character.
 *
 * This function iterates through the command line, respecting single and
 * double quotes. It returns 1 if it finds a '|' character that is not
 * enclosed within any quotes, and 0 otherwise.
 * @param line The command line string to check.
 * @return 1 if an unquoted pipe is found, 0 otherwise.
 */
int	is_unquoted_pipe_present(const char *line)
{
	char	quote_char;

	quote_char = 0;
	while (*line)
	{
		// If we are not inside quotes and we see a quote, enter quote mode
		if (quote_char == 0 && (*line == '\'' || *line == '"'))
			quote_char = *line;
		// If we are inside quotes and we see the matching quote, exit quote mode
		else if (*line == quote_char)
			quote_char = 0;
		// If we are NOT inside quotes and we see a pipe, we found it!
		else if (quote_char == 0 && *line == '|')
			return (1);
		line++;
	}
	// No unquoted pipe was found
	return (0);
}


int	execute_command(char *line, t_env_var *env_list, t_process_data *process_data)
{
	// --- START OF THE FIX ---
	// Check the total number of heredocs for the entire line BEFORE splitting.
	if (count_heredocs(line) > MAX_HEREDOCS)
	{
		ft_error_and_exit("", "maximum here-document count exceeded", 2);
		return (2);
	}
	// --- END OF THE FIX ---

	//if (ft_strchr(line, '|'))
	if (is_unquoted_pipe_present(line))
	{
		printf("pipe is found, calling prepare_and_run_pipeline\n");
		return prepare_and_run_pipeline(line, env_list, process_data);
	}
	else
	{
		t_token **tokens = parse_line(line);
		if (tokens == NULL)
			return (1);
		char **args = expand_and_split_args(tokens, env_list, process_data->last_exit_status);
		// debug
		printf("expand_and_split_args returns: \n");
		// print_array(args);
		free_tokens(tokens);
		if (args == NULL || args[0] == NULL)
		{
			free_split(args);
			return (0); 
		}
		int result = execute_single_command(args, env_list, process_data);
		free_split(args);
		return result;
	}
}
