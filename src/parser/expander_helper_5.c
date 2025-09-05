/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:19:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/05 14:01:19 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void free_partial_strings(char **arr, int upto)
{
    int k;

    k = 0;
    while (k < upto)
    {
        if (arr[k])
            free(arr[k]);
        k++;
    }
}

static int copy_original_arguments(char **dst, char **src, int count)
{
    int i;

    i = 0;
    while (i < count)
    {
        if (src && src[i])
        {
            dst[i] = ft_strdup(src[i]);
            if (!dst[i])
            {
                free_partial_strings(dst, i);
                return (0);
            }
        }
        else
            dst[i] = NULL;
        i++;
    }
    return (1);
}

static int copy_new_arguments(char **dst, char **src, int base, int n)
{
    int j;

    j = 0;
    while (j < n)
    {
        dst[base + j] = ft_strdup(src[j]);
        if (!dst[base + j])
        {
            free_partial_strings(dst, base + j);
            return (0);
        }
        j++;
    }
    return (1);
}

char **append_split_to_final(char **final_args, int *final_count, char **split)
{
    int     split_count;
    char  **new_final;

    split_count = 0;
    while (split && split[split_count] != NULL)
        split_count++;
    new_final = malloc(sizeof(char *) * ((size_t)(*final_count) + (size_t)split_count + 1));
    if (!new_final)
    {
        if (split)
            free_split(split);
        return (NULL);
    }
    if (!copy_original_arguments(new_final, final_args, *final_count)
        || !copy_new_arguments(new_final, split, *final_count, split_count))
    {
        free(new_final);
        if (split)
            free_split(split);
        return (NULL);
    }
    new_final[*final_count + split_count] = NULL;
    if (final_args)
        free_split(final_args);
    if (split)
        free_split(split);
    *final_count += split_count;
    return (new_final);
}

/**
 * @brief Checks if the token had original quotes.
 * 
 * This function checks if the token's value contains any quote characters.
 * @param token The token to check.
 * @return 1 if the token had quotes, 0 otherwise.
 */
static int	had_original_quotes(const t_token *token)
{
	const char	*s;

	if (token == NULL || token->value == NULL)
		return (0);
	s = token->value;
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			return (1);
		s++;
	}
	return (0);
}

/**
 * @brief Processes a token after variable expansion.
 * 
 * This function handles the token based on whether it had original quotes.
 * If it had quotes, it removes them and returns a single-element array.
 * Otherwise, it splits the expanded string on whitespace.
 * @param token The token being processed.
 * @param expanded The expanded string of the token.
 * @return A pointer to the newly allocated array of strings,
 *         or NULL on failure.
 */
char	**process_token(t_token *token, char *expanded)
{
	char	**split;
	char	*unquoted;

	if (had_original_quotes(token))
	{
		unquoted = remove_quotes_and_join(expanded);
		free(expanded);
		expanded = NULL;
		if (unquoted == NULL)
			return (NULL);
		split = (char **)malloc(sizeof(char *) * 2);
		if (split == NULL)
		{
			free(unquoted);
			return (NULL);
		}
		split[0] = unquoted;
		split[1] = NULL;
		return (split);
	}
	split = handle_whitespace_splitting(expanded);
	return (split);
}

int	is_ambiguous_redirect(const char *expanded_value,
				const char *original_value, t_process_data *pdata)
{
	char	**split;
	int		count;

	split = ft_split_whitespace((char *)expanded_value);
	count = 0;
	if (split)
	{
		while (split[count])
			count++;
		free_split(split);
	}
	if (count != 1)
	{
		ft_error((char *)original_value, "ambiguous redirect");
		pdata->last_exit_status = 1;
		pdata->syntax_error = 1;
		return (1);
	}
	return (0);
}
