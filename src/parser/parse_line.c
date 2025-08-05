/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/05 12:15:06 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Helper: skip whitespace
static void skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
		(*s)++;
}

// Helper: duplicate substring
static char *substr_dup(const char *start, size_t len)
{
	char *res = malloc(len + 1);
	if (!res) return NULL;
	for (size_t i = 0; i < len; i++)
		res[i] = start[i];
	res[len] = '\0';
	return res;
}

// Count arguments (words) respecting quotes
static int count_args(const char *s)
{
	int count = 0;
	int in_word = 0;
	char quote = 0;
	while (*s)
	{
		if (!in_word && (*s != ' ' && *s != '\t'))
		{
			in_word = 1;
			count++;
			if (*s == '\'' || *s == '"')
				quote = *s;
		}
		if (in_word)
		{
			if (quote)
			{
				s++;
				while (*s && *s != quote)
					s++;
				if (*s == quote)
					quote = 0;
			}
			while (*s && *s != ' ' && *s != '\t' && !quote)
			{
				if (*s == '\'' || *s == '"')
				{
					quote = *s;
					s++;
					while (*s && *s != quote)
						s++;
					if (*s == quote)
						quote = 0;
				}
				else
					s++;
			}
			in_word = 0;
		}
		if (*s)
			s++;
	}
	return count;
}

// Main parse_line implementation
char **parse_line(char *line)
{
	if (line == NULL || *line == '\0')
		return (NULL);
	int argc = count_args(line);
	char **argv = malloc((argc + 1) * sizeof(char *));
	if (!argv) return NULL;
	int i = 0;
	const char *s = line;
	while (*s && i < argc)
	{
		skip_spaces(&s);
		if (!*s) break;
		const char *start = s;
		char quote = 0;
		size_t len = 0;
		if (*s == '\'' || *s == '"')
		{
			quote = *s;
			s++;
			start = s;
			while (*s && *s != quote)
				s++;
			len = s - start;
			argv[i] = substr_dup(start, len);
			if (*s == quote)
				s++;
		}
		else
		{
			while (s[len] && s[len] != ' ' && s[len] != '\t' && s[len] != '\'' && s[len] != '"')
				len++;
			   if (s[len] == '\'' || s[len] == '"')
			   {
				   // handle mixed quoted/unquoted
				   size_t pre = len;
				   quote = s[len];
				   len++;
				   const char *qstart = s + len;
				   while (s[len] && s[len] != quote)
					   len++;
				   size_t quoted_len = (s[len] == quote) ? (s + len - qstart) : 0;
				   argv[i] = substr_dup(start, pre);
				   char *quoted = substr_dup(qstart, quoted_len);
				   char *joined = malloc(pre + quoted_len + 1);
				   if (argv[i] && quoted && joined)
				   {
					   for (size_t j = 0; j < pre; j++) joined[j] = argv[i][j];
					   for (size_t j = 0; j < quoted_len; j++) joined[pre + j] = quoted[j];
					   joined[pre + quoted_len] = '\0';
					   free(argv[i]);
					   free(quoted);
					   argv[i] = joined;
				   }
				   if (s[len] == quote)
					   len++;
				   s += len;
				   i++;
				   continue;
			   }
			argv[i] = substr_dup(start, len);
			s += len;
		}
		i++;
	}
	argv[i] = NULL;
	return argv;
}



