/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/24 09:34:52 by makhudon      #+#    #+#                 */
/*   Updated: 2025/09/03 14:42:31 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
 * @brief Combines a directory path and a command name into a full command path.
 *
 * This function allocates memory for the combined path, which includes
 * the directory, a '/', the command name, and a null terminator.
 *
 * @param path_dir The directory path.
 * @param cmd The command name.
 * @return A newly allocated string containing the full command path,
 *         or NULL if memory allocation fails.
 */
static char	*combine_cmd_path(const char *path_dir, const char *cmd)
{
	size_t	length;
	char	*full_cmd_path;

	length = ft_strlen(path_dir) + 1 + ft_strlen(cmd) + 1;
	full_cmd_path = malloc(length);
	if (full_cmd_path == NULL)
		return (NULL);
	ft_strlcpy(full_cmd_path, path_dir, length);
	ft_strlcat(full_cmd_path, "/", length);
	ft_strlcat(full_cmd_path, cmd, length);
	return (full_cmd_path);
}

/*
 * @brief Finds the full path of a command by searching through
 		the provided PATH directories.
 *
 * If the command contains a '/', it is treated as an absolute or relative path
 * and returned as is (after duplicating). Otherwise, the function searches
 * through each directory in `path_dirs` to find an executable file matching
 * the command name.
 *
 * @param cmd The command name or path.
 * @param path_dirs An array of directory paths to search, 
 * typically from the PATH environment variable.
 * @return A newly allocated string containing the full path to the command if found,
 *         or NULL if not found or if `cmd` is NULL/empty.
 */
char *find_full_cmd_path(char *cmd, char **path_dirs)
{
    char *full_cmd_path;
    int   i;

    if (!cmd || *cmd == '\0')
        return (NULL);
    if (ft_strchr(cmd, '/'))
        return ft_strdup(cmd);
    i = 0;
    while (path_dirs && path_dirs[i])
    {
        full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
        if (full_cmd_path == NULL)
            ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
        if (access(full_cmd_path, F_OK) == 0)
            return (full_cmd_path);
        free(full_cmd_path);
        i++;
    }
    return (NULL);
}

/*
 * @brief Retrieves the directories listed in the PATH environment variable.
 *
 * This function searches the provided linked list of environment variables
 * for the "PATH" variable, splits its value by the ':' character, and
 * returns an array of directory strings.
 *
 * @param env_list The linked list of environment variables.
 * @return An array of directory strings from the PATH variable,
 *         or NULL if PATH is not found or has no value.
 */
char **find_path_dirs(t_env_var *env_list)
{
    t_env_var	*current;
    char		*path_value;

	current = env_list;
    while (current)
    {
        if (ft_strncmp(current->key, "PATH", 4) == 0 && current->key[4] == '\0')
        {
            path_value = current->value;
            if (path_value == NULL)
                return (NULL);
            return ft_split(path_value, ':');
        }
        current = current->next;
    }
    return (NULL);
}

// ---------- BELOW IS CODE BEFORE CLEAN UP ----------
// ---------- FOR REFERENCE ONLY ----------
// ---------- DO NOT UNCOMMENT ----------
// char	*find_full_cmd_path(char *cmd, char **path_dirs)
// {
// 	char	*full_cmd_path;
// 	int		i;

// 	if (!cmd || *cmd == '\0' || ft_strchr(cmd, '/'))
// 		return (ft_strdup(cmd));
// 	i = 0;
// 	while (path_dirs != NULL && path_dirs[i] != NULL)
// 	{
// 		full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
// 		if (full_cmd_path == NULL)
// 			// error_exit("malloc"); // DEBUG: Print error if malloc fails
// 			ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
// 		if (access(full_cmd_path, X_OK) == 0)
// 			return (full_cmd_path);
// 		free(full_cmd_path);
// 		i++;
// 	}
// 	return (ft_strdup(cmd));
// }


// char *find_full_cmd_path(char *cmd, char **path_dirs)
// {
//     char    *full_cmd_path;
//     int     i;

//     // Check 1: Handle null or empty command string first
//     if (!cmd || *cmd == '\0')
//         return (NULL);

//     // Fix 1: Properly handle absolute and relative paths
//     // If the command contains a slash, treat it as a direct path
//     if (ft_strchr(cmd, '/'))
//     {
//         if (access(cmd, X_OK) == 0)
//             return (ft_strdup(cmd));
//         return (NULL); // The command exists but is not executable, or the path is invalid
//     }

//     // Fix 2: Search PATH directories only if path_dirs is not NULL
//     i = 0;
//     while (path_dirs && path_dirs[i])
//     {
//         full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
//         if (full_cmd_path == NULL)
//             ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
//         if (access(full_cmd_path, X_OK) == 0)
//             return (full_cmd_path);
//         free(full_cmd_path);
//         i++;
//     }

//     // Fix 3: Return NULL if the command is not found in PATH
//     return (NULL);
// }

// char *find_full_cmd_path(char *cmd, char **path_dirs)
// {
//     char *full_cmd_path;
//     int i;

//     if (!cmd || *cmd == '\0')
//         return (NULL);

//     if (ft_strchr(cmd, '/'))
//     {
//         if (access(cmd, F_OK) != 0)
//             return (NULL); // File does not exist
//         if (access(cmd, X_OK) != 0)
//         {
//             errno = EACCES; // File exists but not executable
//             return (ft_strdup(cmd));
//         }
//         return (ft_strdup(cmd)); // File exists and executable
//     }

//     i = 0;
//     while (path_dirs && path_dirs[i])
//     {
//         full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
//         if (full_cmd_path == NULL)
//             ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
//         if (access(full_cmd_path, X_OK) == 0)
//             return (full_cmd_path);
//         free(full_cmd_path);
//         i++;
//     }

//     return (NULL);
// }

// char *find_full_cmd_path(char *cmd, char **path_dirs)
// {
//     char    *full_cmd_path;
//     int     i;

//     if (!cmd || *cmd == '\0')
//         return (NULL);

//     /* explicit path */
//     if (ft_strchr(cmd, '/'))
//     {
//         if (access(cmd, F_OK) == 0)
//             return (ft_strdup(cmd));   /* exists: executor will check X_OK/dir */
//         return (NULL);                  /* doesn’t exist */
//     }

//     /* PATH search: return on F_OK, not X_OK */
//     i = 0;
//     while (path_dirs && path_dirs[i])
//     {
//         full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
//         if (full_cmd_path == NULL)
//             ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);

//         if (access(full_cmd_path, F_OK) == 0)
//             return (full_cmd_path);

//         free(full_cmd_path);
//         i++;
//     }
//     return (NULL);
// }

// char *find_full_cmd_path(char *cmd, char **path_dirs)
// {
//     char *full_cmd_path;
//     int   i;

//     if (!cmd || *cmd == '\0')
//         return (NULL);

//     // --- Case 1: If cmd has a slash → treat as literal path
//     if (ft_strchr(cmd, '/'))
//     {
//         if (access(cmd, F_OK) == 0)
//             return ft_strdup(cmd);
//         return (NULL);
//     }

//     // --- Case 2: If PATH exists → search all directories
//     i = 0;
//     if (path_dirs)
//     {
//         while (path_dirs[i])
//         {
//             full_cmd_path = combine_cmd_path(path_dirs[i], cmd);
//             if (full_cmd_path == NULL)
//                 ft_error_and_exit("malloc", strerror(errno), EXIT_FAILURE);
//             if (access(full_cmd_path, F_OK) == 0)
//                 return (full_cmd_path);
//             free(full_cmd_path);
//             i++;
//         }
//     }
//     else
//     {
//         // --- Case 3: PATH is NULL → fallback to ./cmd (like Bash)
//         if (access(cmd, F_OK) == 0)
//             return (ft_strdup(cmd));
//     }

//     return (NULL);
// }

