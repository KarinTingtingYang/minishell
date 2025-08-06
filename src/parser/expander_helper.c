/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/06 09:14:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// Helper function to check whitespace (no isspace)
static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

// Count words separated by whitespace
static int count_words(const char *s)
{
    int count = 0;
    int in_word = 0;

    while (*s)
    {
        if (!is_space(*s) && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (is_space(*s))
            in_word = 0;
        s++;
    }
    return count;
}

// Split string on whitespace, returns NULL-terminated array of strings
char **ft_split_whitespace(char *s)
{
    int words = count_words(s);
    char **result;
    int i = 0;

    result = malloc((words + 1) * sizeof(char *));
    if (!result)
        return NULL;

    while (*s)
    {
        while (*s && is_space(*s))
            s++;
        if (!*s)
            break;

        const char *start = s;
        size_t len = 0;
        while (*s && !is_space(*s))
        {
            s++;
            len++;
        }
        result[i++] = ft_strndup(start, len);
    }
    result[i] = NULL;
    return result;
}

char **ft_split_single(const char *s)
{
    char **res = malloc(sizeof(char *) * 2);
    if (!res) return NULL;
    res[0] = ft_strdup(s);
    res[1] = NULL;
    return res;
}
