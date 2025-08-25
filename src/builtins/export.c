/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:03:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/25 10:18:19 by makhudon         ###   ########.fr       */
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

	sorted_list = duplicate_env_list(env_list);
	bubble_sort_env_list(sorted_list);
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
	free_env(sorted_list);
}

static char	*strip_syntactic_quotes(const char *s)
{
	size_t	i;
	size_t	w;
	size_t	len;
	char	quote;
	char 	*out;

    if (!s) return NULL;
    len = ft_strlen(s);
    out = (char *)malloc(len + 1);
    if (!out) return NULL;

    i = 0; w = 0; quote = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
        {
            if (quote == 0) { quote = s[i++]; continue; }
            if (quote == s[i]) { quote = 0; i++; continue; }
        }
        out[w++] = s[i++];
    }
    out[w] = '\0';
    return out;
}

static int export_variable(const char *arg, t_env_var *env_list)
{
    char        *key;
    char        *equal_sign;
    char        *raw;        /* text after '=' as typed (with quotes) */
    char        *expanded;
    char        *clean;
    t_env_var   *existing_var;
    int         append;

    append = 0;
    equal_sign = ft_strchr(arg, '=');

    /* -------- case: export NAME (no assignment) -------- */
    if (equal_sign == NULL)
    {
        if (!is_valid_identifier((char *)arg))
        {
            ft_error("export", "not a valid identifier");
            return 1;
        }
        existing_var = find_env_var((char *)arg, env_list);
        if (existing_var == NULL)
        {
            key = ft_strdup(arg);
            if (!key) return 1;

            /* If add_env_var is NULL-safe, prefer NULL; otherwise "" avoids segfaults */
            /* add_env_var(key, NULL, env_list); */
            add_env_var(key, "", env_list);

            free(key);
        }
        return 0;
    }

    /* -------- parse key and detect NAME+= -------- */
    if (equal_sign > arg && *(equal_sign - 1) == '+')
    {
        append = 1;
        key = ft_substr(arg, 0, (size_t)(equal_sign - arg - 1));
    }
    else
        key = ft_substr(arg, 0, (size_t)(equal_sign - arg));

    if (!key || !is_valid_identifier(key))
    {
        ft_error("export", "not a valid identifier");
        if (key) free(key);
        return 1;
    }

    /* -------- expand RHS and strip syntactic quotes -------- */
    raw = ft_strdup(equal_sign + 1);
    if (!raw) { free(key); return 1; }

    /* Use 0 for last_exit_status to keep signature 2-arg and compile everywhere */
    expanded = expand_variables(raw, env_list, 0, NO_QUOTE);
    free(raw);
    if (!expanded) { free(key); return 1; }

    clean = strip_syntactic_quotes(expanded);
    free(expanded);
    if (!clean) { free(key); return 1; }

    /* -------- store/append -------- */
    existing_var = find_env_var(key, env_list);
    if (existing_var)
    {
        if (append && existing_var->value)
        {
            char *joined = ft_strjoin(existing_var->value, clean);
            if (!joined) { free(clean); free(key); return 1; }
            free(existing_var->value);
            existing_var->value = joined;
            free(clean);
        }
        else
        {
            free(existing_var->value);
            existing_var->value = clean;  /* take ownership */
        }
    }
    else
    {
        add_env_var(key, clean, env_list);
        /* if add_env_var duplicates internally, uncomment: */
        free(clean);
    }

    free(key);
    return 0;
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
