/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 13:55:56 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/22 12:45:56 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

extern char **environ;
extern volatile sig_atomic_t g_child_running; // Declare extern for global flag

void handle_execve_error(char *cmd_path, char **args, char **path_dirs)
{
	int exit_status;
	(void)args;
	(void)path_dirs; // Unused parameters, can be removed if not needed

	if (errno == ENOENT)
	{
		write(STDERR_FILENO, cmd_path, ft_strlen(cmd_path));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit_status = 127;
	}
	else
	{
		perror(cmd_path);
		exit_status = 1;
	}
	// free_split(path_dirs);
	// free_split(args);
	// if (args != NULL && is_malloced(args))
	// 	free_split(args);
	free(cmd_path);
	exit(exit_status);
}

void execute_cmd(char *cmd_path, char **args, char **path_dirs)
{
	
    // Reset signal handlers to default for the child process
    reset_child_signal_handlers(); //
	if (cmd_path == NULL)
	{
		free_split(path_dirs);
		// free_split(args);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	execve(cmd_path, args, environ);
	handle_execve_error(cmd_path, args, path_dirs);
}

static int execute_single_command(char *line, char **envp)
{
	(void)envp; // envp is not used in this function, but can be used for future enhancements
	char	**args;
	char	**path_dirs;
	char	*cmd_path;
	pid_t	pid;
	// int		status; --> moved to helper function wait_for_child_and_handle_status() in signal.c
	int		exit_code; // new variable to hold exit code returned from helper function 
					   // wait_for_child_and_handle_status()

	if (line == NULL || *line == '\0')
		return 0;

	args = ft_split(line, ' ');
	if (args == NULL || args[0] == NULL)
	{
		free_split(args);
		return (0);
	}
	path_dirs = find_path_dirs(environ);
	
	if (!path_dirs)
    	printf("Error: PATH variable not found\n");
	else 
	{
    	printf("PATH dirs:\n");
    	for (int i = 0; path_dirs[i] != NULL; i++)
        	printf("  %s\n", path_dirs[i]);
	}
	cmd_path = find_full_cmd_path(args[0], path_dirs);
	
	printf("Trying: %s\n", cmd_path);

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free_split(path_dirs);
		free_split(args);
		free(cmd_path);
		return (-1);
	}
	else if (pid == 0)
	{
		execute_cmd(cmd_path, args, path_dirs);
	}
	else
	{
		// moved wpid and exit code check to helper function wait_for_child_and_handle_status()
		exit_code = wait_for_child_and_handle_status(pid);
		free_split(path_dirs);
		free_split(args);
		free(cmd_path);
		return (exit_code);
	}
	return 0;
}

// int execute_command(char *line, char **envp)
// {
// 	if (ft_strchr(line, '|'))
// 	{
// 		char **parts = ft_split(line, '|');
// 		if (!parts || !parts[0] || !parts[1])
// 		{
// 			ft_putstr_fd("Error: invalid pipe syntax\n", STDERR_FILENO);
// 			if (parts)
// 				free_split(parts);
// 			return 1;
// 		}
// 		int status = run_pipex(parts[0], parts[1], envp);
// 		free_split(parts);
// 		return status;
// 	}
// 	else
// 		return execute_single_command(line, envp);
// }

static int count_commands(char **parts)
{
    int count = 0;
    while (parts[count])
        count++;
    return count;
}

static int create_commands_recursive(t_command **cmds, char **parts, int index, int count, char **envp)
{
    if (index >= count)
        return 1; // success base case

    cmds[index] = create_command(parts[index], envp);
    if (!cmds[index])
        return 0; // failure

    return create_commands_recursive(cmds, parts, index + 1, count, envp);
}

static void free_commands_recursive(t_command **cmds, int index, int count)
{
    if (index >= count)
        return;

    free_command(cmds[index]);
    free_commands_recursive(cmds, index + 1, count);
}

int execute_command(char *line, char **envp)
{
    if (ft_strchr(line, '|'))
    {
        char **parts = ft_split(line, '|');
        if (!parts || !parts[0])
        {
            ft_putstr_fd("Error: invalid pipe syntax\n", STDERR_FILENO);
            if (parts)
                free_split(parts);
            return 1;
        }

        int count = count_commands(parts);

        t_command **cmds = malloc(sizeof(t_command *) * (count + 1));
        if (!cmds)
        {
            free_split(parts);
            return 1;
        }
        cmds[count] = NULL;

        if (!create_commands_recursive(cmds, parts, 0, count, envp))
        {
            free_commands_recursive(cmds, 0, count);
            free(cmds);
            free_split(parts);
            return 1;
        }

        int status = execute_pipeline(cmds, count, envp);

        free_commands_recursive(cmds, 0, count);
        free(cmds);
        free_split(parts);

        return status;
    }
    else
    {
        return execute_single_command(line, envp);
    }
}

