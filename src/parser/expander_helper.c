/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 14:41:57 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

static int count_words(const char *s)
{
    int count = 0;
    int in_word = 0;

    while (*s != '\0')
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

/**
 * @brief Splits a string into words based on whitespace.
 * 
 * This function allocates memory for an array of strings, where each string
 * is a word from the input string. It handles multiple spaces and trims
 * leading/trailing whitespace.
 * @param s The input string to split.
 * @return A NULL-terminated array of strings (words), or NULL on failure.
 */
char **ft_split_whitespace(char *s)
{
    int words = count_words(s);
    char **result;
    int i = 0;

    result = malloc((words + 1) * sizeof(char *));
    if (result == NULL)
        return NULL;
    while (*s)
    {
        while (*s && is_space(*s))
            s++;
        if (*s == '\0')
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
    if (res == NULL)
		return NULL; 
    res[0] = ft_strdup(s);
    res[1] = NULL;
    return res;
}
