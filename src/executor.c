/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 09:40:10 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 10:13:48 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"

int execute_command(char *line, char **envp)
{
	pid_t	pid;
	int		status;
	(void)line; // unused for now
	char *cmd[] = {"/bin/ls", "-l", NULL};  // hardcoded command

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	else if (pid == 0)
	{
		execve(cmd[0], cmd, envp);
		perror("execve");
		exit(127);
	}
	else
	{
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("waitpid");
			return (-1);
		}
		if (WIFEXITED(status))
			printf("Child exited with status %d\n", WEXITSTATUS(status));
	}
	return (0);
}
