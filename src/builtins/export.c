/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/30 13:03:36 by makhudon      #+#    #+#                 */
/*   Updated: 2025/08/12 14:40:31 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// REPLACED QSORT WITH BUBBLE SORT AS QSORT IS NOT ALLOWED IN THIS PROJECT

/**
 * @brief Checks if a string is a valid identifier for an environment variable.
 * (Starts with a letter or underscore, followed by letters, digits, or
 * underscores).
 *
 * @param str The string to check.
 * @return 1 if valid, 0 otherwise.
 */
int	is_valid_identifier(const char *str)
{
	if (!str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	str++;
	while (*str)
	{
		if (!ft_isalnum(*str) && *str != '_')
			return (0);
		str++;
	}
	return (1);
}

// static int cmp_env(const void *a, const void *b)
// {
//     const char *str1 = *(const char **)a;
//     const char *str2 = *(const char **)b;

//     // Compare until '=' or end of string
//     int i = 0;
//     while (str1[i] && str2[i] && str1[i] != '=' && str2[i] != '=')
//     {
//         if (str1[i] != str2[i])
//             return ((unsigned char)str1[i] - (unsigned char)str2[i]);
//         i++;
//     }
//     if (str1[i] == '=' && str2[i] == '=')
//         return 0;
//     if (str1[i] == '=')
//         return -1;
//     if (str2[i] == '=')
//         return 1;
//     return 0;
// }

/**
 * @brief Sorts an environment linked list alphabetically using bubble sort
 * with ft_strncmp.
 * @param start The head of the list to sort.
 */
static void	bubble_sort_env_list(t_env_var *start)
{
	int			swapped;
	t_env_var	*ptr1;
	char		*temp_key;
	char		*temp_value;
	size_t		len1;
	size_t		len2;
	size_t		n;

	if (start == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = start;
		while (ptr1->next != NULL)
		{
			len1 = ft_strlen(ptr1->key);
			len2 = ft_strlen(ptr1->next->key);
			n = (len1 > len2) ? len1 : len2;
			if (ft_strncmp(ptr1->key, ptr1->next->key, n) > 0)
			{
				// Swap the data of the two nodes
				temp_key = ptr1->key;
				temp_value = ptr1->value;
				ptr1->key = ptr1->next->key;
				ptr1->value = ptr1->next->value;
				ptr1->next->key = temp_key;
				ptr1->next->value = temp_value;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
	}
}


/**
 * @brief Duplicates, sorts, and prints the environment list for the 'export'
 * command.
 * @param env_list The original environment list.
 */
static void	display_export(t_env_var *env_list)
{
	t_env_var	*sorted_list;
	t_env_var	*current;

	// 1. Duplicate the list so we don't alter the original
	sorted_list = duplicate_env_list(env_list);

	// if (sorted_list == NULL) // DEBUG: Check if duplication failed
    //     return (1);
		
	// 2. Sort the duplicated list
	bubble_sort_env_list(sorted_list);

	// 3. Print the sorted list in the required format
	current = sorted_list;
	while (current)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(current->key, STDOUT_FILENO);
		if (current->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(current->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		current = current->next;
	}

	// 4. Free the duplicated list to prevent memory leaks
	free_env(sorted_list);
}

/**
 * @brief Exports a variable to the environment list.
 * 
 * This function checks if the variable is a valid identifier, and if so,
 * adds or updates it in the environment list. If the variable is invalid,
 * it prints an error message.
 * @param arg The argument to export, expected to be in the format "KEY=VALUE".
 * @param env_list The environment list where the variable should be added.
 * @return 0 on success, 1 on error (invalid identifier).
 */
// static int	export_variable(const char *arg, t_env_var *env_list)
// {
// 	char		*key;
// 	char		*value;
// 	char		*equal_sign;
// 	t_env_var	*existing_var;

// 	equal_sign = ft_strchr(arg, '=');
// 	if (!equal_sign) // If there's no '=', we do nothing for now.
// 		return (0);
// 	key = ft_substr(arg, 0, equal_sign - arg);
// 	printf("export_variable: key = %s\n", key); // DEBUG: Print the key being exported
// 	if (!is_valid_identifier(key))
// 	{
// 		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
// 		ft_putstr_fd(arg, STDERR_FILENO);
// 		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
// 		free(key);
// 		return (1); // Return error
// 	}
// 	value = ft_strdup(equal_sign + 1);
// 	printf("export_variable: value = %s\n", value); // DEBUG: Print the value being exported
// 	existing_var = find_env_var(key, env_list);
// 	if (existing_var)
// 	{
// 		free(existing_var->value); // Free the old value
// 		existing_var->value = value; // Assign the new one
// 	}
// 	else
// 	{
// 		add_env_var(key, value, env_list);
// 	}
// 	free(key);
// 	return (0);
// }

static int	export_variable(const char *arg, t_env_var *env_list)
{
	char		*key;
	char		*value;
	char		*equal_sign;
	t_env_var	*existing_var;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign) // If there's no '=', we do nothing for now.
		return (0);

	// Extract key before '='
	key = ft_substr(arg, 0, equal_sign - arg);
	printf("export_variable: key = %s\n", key); // DEBUG

	if (!is_valid_identifier(key))
	{
		// ft_putstr_fd("minishell: export: `", STDERR_FILENO); // DEBUG: Print error if key is invalid
		// ft_putstr_fd(arg, STDERR_FILENO); // DEBUG: Print the invalid argument
		// ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO); // DEBUG: Print the error message
		ft_error("export", "not a valid identifier");
		free(key);
		return (1); // Return error
	}

	// Duplicate value part (after '=')
	value = ft_strdup(equal_sign + 1);

	// ✅ Strip surrounding quotes if both first and last chars are matching quotes
	if (value && ((value[0] == '"' || value[0] == '\'') && value[ft_strlen(value) - 1] == value[0]))
	{
		char *unquoted = ft_substr(value, 1, ft_strlen(value) - 2);
		free(value);
		value = unquoted;
	}

	printf("export_variable: value = %s\n", value); // DEBUG

	// Replace or add variable
	existing_var = find_env_var(key, env_list);
	if (existing_var)
	{
		free(existing_var->value); // Free old value
		existing_var->value = value;
	}
	else
	{
		add_env_var(key, value, env_list);
	}

	free(key);
	free(value); // MEMORY LEAK FIX: Free the value after use
	key	= NULL; // Avoid dangling pointer
	value = NULL; // Avoid dangling pointer
	return (0);
}


int	run_export(t_env_var *env_list, char **args)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (args[1] == NULL)
	{
		display_export(env_list);
		return (0);
	}
	while (args[i])
	{
		if (export_variable(args[i], env_list) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}

// int run_export(t_env_var *env_list, char **args)
// {
//     if (args[1] != NULL)
//     {
//         // For now, ignore export with arguments
//         return 1;
//     }

//     char **env_arr = env_list_to_array(env_list);
//     if (!env_arr)
//         return 1;

//     int len = 0;
//     while (env_arr[len])
//         len++;

//     qsort(env_arr, len, sizeof(char *), cmp_env);

//     for (int i = 0; i < len; i++)
//     {
//         char *equal_sign = ft_strchr(env_arr[i], '=');
//         if (equal_sign)
//         {
//             *equal_sign = '\0';
//             printf("declare -x %s=\"%s\"\n", env_arr[i], equal_sign + 1);
//             *equal_sign = '=';
//         }
//         else
//         {
//             printf("declare -x %s\n", env_arr[i]);
//         }
//         free(env_arr[i]);
//     }
//     free(env_arr);
//     return 0;
// }
