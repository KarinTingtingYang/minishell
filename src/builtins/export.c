/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 13:03:36 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/29 19:50:30 by makhudon         ###   ########.fr       */
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
// static void	display_export(t_env_var *env_list)
// {
// 	t_env_var	*sorted_list;
// 	t_env_var	*current;

// 	sorted_list = duplicate_env_list(env_list);
// 	bubble_sort_env_list(sorted_list);
// 	current = sorted_list;
// 	while (current)
// 	{
// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(current->key, STDOUT_FILENO);
// 		if (current->value)
// 		{
// 			ft_putstr_fd("=\"", STDOUT_FILENO);
// 			ft_putstr_fd(current->value, STDOUT_FILENO);
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		}
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		current = current->next;
// 	}
// 	free_env(sorted_list);
// }

// static void	display_export(t_env_var *env_list)
// {
// 	t_env_var	*sorted_list;
// 	t_env_var	*cur;

// 	sorted_list = duplicate_env_list(env_list);
// 	bubble_sort_env_list(sorted_list);

// 	cur = sorted_list;
// 	while (cur)
// 	{
// 		int j;

// 		ft_putstr_fd("declare -x ", STDOUT_FILENO);
// 		ft_putstr_fd(cur->key, STDOUT_FILENO);

// 		if (cur->value != NULL)
// 		{
// 			ft_putstr_fd("=\"", STDOUT_FILENO);
// 			/* minimal bash-like escaping for ", \ and $ */
// 			j = 0;
// 			while (cur->value[j] != '\0')
// 			{
// 				if (cur->value[j] == '"' || cur->value[j] == '\\' || cur->value[j] == '$')
// 					ft_putchar_fd('\\', STDOUT_FILENO);
// 				ft_putchar_fd(cur->value[j], STDOUT_FILENO);
// 				j++;
// 			}
// 			ft_putstr_fd("\"", STDOUT_FILENO);
// 		}
// 		ft_putstr_fd("\n", STDOUT_FILENO);
// 		cur = cur->next;
// 	}
// 	free_env(sorted_list);
// }

/* local helper */
static void put_escaped_export_value(const char *s)
{
    size_t i;

    i = 0;
    while (s && s[i])
    {
        if (s[i] == '"' || s[i] == '\\' || s[i] == '$')
            ft_putchar_fd('\\', STDOUT_FILENO);
        ft_putchar_fd(s[i], STDOUT_FILENO);
        i++;
    }
}

static void display_export(t_env_var *env_list)
{
    t_env_var *sorted_list;
    t_env_var *cur;

    sorted_list = duplicate_env_list(env_list);
    bubble_sort_env_list(sorted_list);

    cur = sorted_list;
    while (cur)
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(cur->key, STDOUT_FILENO);
        if (cur->value != NULL)                 /* print only if assigned */
        {
            ft_putstr_fd("=\"", STDOUT_FILENO);
            put_escaped_export_value(cur->value);
            ft_putstr_fd("\"", STDOUT_FILENO);
        }
        ft_putstr_fd("\n", STDOUT_FILENO);
        cur = cur->next;
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
            error_with_backticked_arg("export", arg, "not a valid identifier");
            return 1;
        }
        existing_var = find_env_var((char *)arg, env_list);
        if (existing_var == NULL)
        {
            // char *empty;

            key = ft_strdup(arg);
            if (!key)
                return 1;
            // empty = ft_strdup("");
            // if (!empty)
            // {
            //     free(key);
            //     return 1;
            // }
            // add_env_var(key, empty, env_list);
			add_env_var(key, NULL, env_list);
            /* if add_env_var duplicates internally, you may free(empty) here */
            /* free(empty); */
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
        error_with_backticked_arg("export", arg, "not a valid identifier");
        if (key) free(key);
        return 1;
    }

    /* -------- expand RHS and strip syntactic quotes -------- */
    raw = ft_strdup(equal_sign + 1);
    if (!raw) { free(key); return 1; }

    /* If you track last_exit_status/quote type, pass them; here kept simple */
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
            char *joined;

            joined = ft_strjoin(existing_var->value, clean);
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
        /* free(clean); */
    }

    free(key);
    return 0;
}

// static int  export_variable(const char *arg, t_env_var *env_list)
// {
//     char      *key;
//     char      *eq;
//     char      *clean;      /* те, що запишемо, БЕЗ повторних експансій */
//     t_env_var *exist;
//     int        append;

//     append = 0;
//     eq = ft_strchr(arg, '=');

//     /* --- export NAME (без '=') --- */
//     if (eq == NULL)
//     {
//         if (!is_valid_identifier((char *)arg))
//         {
//             ft_error("export", "not a valid identifier");
//             return 1;
//         }
//         exist = find_env_var((char *)arg, env_list);
//         if (exist == NULL)
//         {
//             key = ft_strdup(arg);
//             if (!key) return 1;
//             /* важливо: створюємо НЕпризначене значення */
//             add_env_var(key, NULL, env_list);
//             free(key);
//         }
//         return 0;
//     }

//     /* --- розбір ключа + NAME+= --- */
//     if (eq > arg && *(eq - 1) == '+')
//         key = ft_substr(arg, 0, (size_t)(eq - arg - 1)), append = 1;
//     else
//         key = ft_substr(arg, 0, (size_t)(eq - arg));

//     if (!key || !is_valid_identifier(key))
//     {
//         ft_error("export", "not a valid identifier");
//         if (key) free(key);
//         return 1;
//     }

//     /* --- БІЛЬШЕ НЕ ЕКСПАНДИМО! те, що прийшло з parser/expander — фінальне --- */
//     clean = ft_strdup(eq + 1);
//     if (!clean) { free(key); return 1; }

//     exist = find_env_var(key, env_list);
//     if (exist)
//     {
//         if (append && exist->value)
//         {
//             char *joined = ft_strjoin(exist->value, clean);
//             if (!joined) { free(clean); free(key); return 1; }
//             free(exist->value);
//             exist->value = joined;
//             free(clean);
//         }
//         else
//         {
//             if (exist->value) free(exist->value);
//             exist->value = clean; /* беремо у власність */
//         }
//     }
//     else
//     {
//         add_env_var(key, clean, env_list);
//         /* якщо add_env_var копіює — free(clean); якщо бере у власність — лишаємо */
//     }

//     free(key);
//     return 0;
// }

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
