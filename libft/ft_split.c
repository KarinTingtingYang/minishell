/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:29:35 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 12:47:23 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

// What ft_split must do:
// It takes:
// 	•	A string s
// 	•	A character c (delimiter)
// And returns:
// 	•	An array of strings (i.e. char **) that are substrings
// 		from s, split wherever c appears.
// 	•	The array ends with a NULL pointer.
// 	•	Uses only malloc() and free().

static void	free_all(char **array, int count)
{
	int	i;

	i = count - 1;
	while (i >= 0)
	{
		free(array[i]);
		i--;
	}
	free(array);
}

static int	count_words(const char *s, char delim)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] != '\0')
	{
		if (s[i] != delim)
		{
			count++;
			while (s[i] != '\0' && s[i] != delim)
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	*copy_word(const char *start, int length)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(length + 1);
	if (word == NULL)
		return (NULL);
	while (i < length)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**split_words(const char *s, char delim, int word_count)
{
	char		**result;
	int			word_index;
	const char	*word_start;

	result = malloc((word_count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	word_index = 0;
	while (*s && word_index < word_count)
	{
		while (*s == delim)
			s++;
		word_start = s;
		while (*s && *s != delim)
			s++;
		result[word_index] = copy_word(word_start, (s - word_start));
		if (result[word_index] == NULL)
		{
			free_all(result, word_index);
			return (NULL);
		}
		word_index++;
	}
	result[word_index] = NULL;
	return (result);
}

char	**ft_split(const char *s, char c)
{
	int		word_count;
	char	**result;

	if (s == NULL)
		return (NULL);
	word_count = count_words(s, c);
	result = split_words(s, c, word_count);
	return (result);
}
