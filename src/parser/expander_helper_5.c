/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_helper_5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 15:19:17 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/08 09:48:56 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Duplicates a specified number of strings from a source array.
 *
 * This function creates a deep copy of `count` strings from the `src` array
 * into the `dst` array. If memory allocation for any string fails, it
 * immediately frees all previously copied strings to prevent memory leaks.
 *
 * @param dst The destination array for the new strings.
 * @param src The source array of strings to copy.
 * @param count The number of strings to duplicate.
 * @return Returns 1 on success, or 0 if a memory allocation fails.
 */
static int	copy_original_arguments(char **dst, char **src, int count)
{
	int	i;

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

/**
 * @brief Duplicates `n` strings from `src` to `dst` at a given offset.
 *
 * This function deep-copies `n` strings from the `src` array to the `dst`
 * array, starting at the `base` index. It includes error handling to free
 * memory on failure.
 *
 * @param dst The destination array.
 * @param src The source array.
 * @param base The starting index in `dst`.
 * @param n The number of strings to copy.
 * @return 1 on success, or 0 on memory allocation failure.
 */
static int	copy_new_arguments(char **dst, char **src, int base, int n)
{
	int	j;

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

/**
 * @brief Counts the number of strings in a null-terminated array.
 * @param split The array of strings to count.
 * @return The number of strings in the array.
 */
static int	get_split_count(char **split)
{
	int	count;

	count = 0;
	while (split && split[count] != NULL)
		count++;
	return (count);
}

/**
 * @brief Handles the main copying logic and error cleanup with
 *        a 4-argument limit.
 *
 * This function now calculates the `split_count` internally to meet the
 * 4-argument limit. It attempts to copy both the original and new arguments
 * into the newly allocated array. If any memory allocation fails, it
 * cleans up the partially allocated memory and returns 0.
 *
 * @param new_final The newly allocated destination array.
 * @param final_args The original array of arguments.
 * @param final_count The count of original arguments.
 * @param split The array of new strings to append.
 * @return 1 on success, 0 on memory allocation failure.
 */
static int	copy_and_check_for_errors(char **new_final, char **final_args,
									int final_count, char **split)
{
	int	split_count;

	split_count = get_split_count(split);
	if (!copy_original_arguments(new_final, final_args, final_count)
		|| !copy_new_arguments(new_final, split, final_count, split_count))
	{
		free(new_final);
		if (split)
			free_split(split);
		return (0);
	}
	return (1);
}

/**
 * @brief Appends a new array of strings to an existing one.
 * @param final_args The original array of arguments.
 * @param final_count A pointer to the count of original arguments.
 * @param split The array of new strings to append.
 * @return A pointer to the newly created combined array, or NULL on failure.
 */
char	**append_split_to_final(char **final_args, int *final_count,
										char **split)
{
	int		split_count;
	char	**new_final;

	split_count = get_split_count(split);
	new_final = malloc(sizeof(char *)
			* ((size_t)(*final_count) + (size_t)split_count + 1));
	if (!new_final)
	{
		if (split)
			free_split(split);
		return (NULL);
	}
	if (!copy_and_check_for_errors(new_final, final_args, *final_count, split))
		return (NULL);
	new_final[*final_count + split_count] = NULL;
	if (final_args)
		free_split(final_args);
	if (split)
		free_split(split);
	*final_count += split_count;
	return (new_final);
}
