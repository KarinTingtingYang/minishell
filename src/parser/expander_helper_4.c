/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 10:39:19 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/18 13:02:45 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	copy_new_arguments(char **new_final, char **split,
									int final_count, int split_count)
{
	int	j;

	j = 0;
	while (j < split_count)
	{
		new_final[final_count + j] = split[j];
		j++;
	}
}

void	copy_original_arguments(char **new_final,
										char **final_args, int final_count)
{
	int	i;

	i = 0;
	while (i < final_count)
	{
		new_final[i] = final_args[i];
		i++;
	}
}

// static void	copy_string_without_quotes(char *result, char *str)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	j = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != '\'' && str[i] != '"')
// 			result[j++] = str[i];
// 		i++;
// 	}
// 	result[j] = '\0';
// }

// static size_t	calculate_unquoted_length(const char *str)
// {
// 	size_t	len;
// 	int		i;

// 	printf("calculate_unquoted_length receives [%s]\n", str);
// 	len = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] != '\'' && str[i] != '"')
// 			len++;
// 		i++;
// 	}
// 	printf("calculate_unquoted_length returns %zu\n", len);
// 	return (len);
// }

// char	*remove_quotes_and_join(char *str)
// {
// 	char	*result;
// 	size_t	len;

// 	if (str == NULL)
// 		return (NULL);
// 	len = calculate_unquoted_length(str);
// 	result = malloc(sizeof(char) * (len + 1));
// 	if (result == NULL)
// 	{
// 		// free(str);
// 		return (NULL);
//     }
//     copy_string_without_quotes(result, str);
//     // free(str);
//     printf("remove_quotes_and_join returns [%s]\n", result);
//     return (result);
// }

static void copy_string_without_quotes(char *result, char *str)
{
    int     i;
    int     j;
    char    quote_char;

    i = 0;
    j = 0;
    quote_char = 0;
    while (str[i] != '\0')
    {
        if (quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
            quote_char = str[i];
        else if (str[i] == quote_char)
            quote_char = 0;
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
}

static size_t calculate_unquoted_length(const char *str)
{
    size_t  len;
    char    quote_char;
    int     i;

    printf("calculate_unquoted_length receives [%s]\n", str);
    len = 0;
    quote_char = 0;
    i = 0;
    while (str[i] != '\0')
    {
        if (quote_char == 0 && (str[i] == '\'' || str[i] == '"'))
            quote_char = str[i];
        else if (str[i] == quote_char)
            quote_char = 0;
        else
            len++;
        i++;
    }
    printf("calculate_unquoted_length returns %zu\n", len);
    return (len);
}

// Case: unquoted token with possible embedded quotes
char *remove_quotes_and_join(char *str)
{
    char    *result;
    size_t  len;

    if (str == NULL)
        return (NULL);
    len = calculate_unquoted_length(str);
    result = malloc(sizeof(char) * (len + 1));
    if (!result)
        return (NULL);
    copy_string_without_quotes(result, str);
    printf("remove_quotes_and_join returns [%s]\n", result);
    return (result);
}

char *remove_outer_quotes(char *str)
{
	size_t len = ft_strlen(str);
	char *res;

	if (len >= 2 && ((str[0] == '\'' && str[len-1] == '\'') ||
	                 (str[0] == '"'  && str[len-1] == '"')))
	{
		if (len == 2)
			res = ft_strdup("");
		else
			res = ft_substr(str, 1, len - 2);
		if (!res) { free(str); return (NULL); }
		free(str);
		return (res);
	}
	return (str);
}
