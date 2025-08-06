/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/06 13:41:37 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// // Helper: skip whitespace
// static void skip_spaces(const char **s)
// {
// 	while (**s && (**s == ' ' || **s == '\t'))
// 		(*s)++;
// }

// // Helper: duplicate substring
// static char *substr_dup(const char *start, size_t len)
// {
// 	char *res = malloc(len + 1);
// 	if (!res) return NULL;
// 	for (size_t i = 0; i < len; i++)
// 		res[i] = start[i];
// 	res[len] = '\0';
// 	return res;
// }

// // Count arguments (words) respecting quotes
// static int count_args(const char *s)
// {
// 	int count = 0;
// 	int in_word = 0;
// 	char quote = 0;
// 	while (*s)
// 	{
// 		if (!in_word && (*s != ' ' && *s != '\t'))
// 		{
// 			in_word = 1;
// 			count++;
// 			if (*s == '\'' || *s == '"')
// 				quote = *s;
// 		}
// 		if (in_word)
// 		{
// 			if (quote)
// 			{
// 				s++;
// 				while (*s && *s != quote)
// 					s++;
// 				if (*s == quote)
// 					quote = 0;
// 			}
// 			while (*s && *s != ' ' && *s != '\t' && !quote)
// 			{
// 				if (*s == '\'' || *s == '"')
// 				{
// 					quote = *s;
// 					s++;
// 					while (*s && *s != quote)
// 						s++;
// 					if (*s == quote)
// 						quote = 0;
// 				}
// 				else
// 					s++;
// 			}
// 			in_word = 0;
// 		}
// 		if (*s)
// 			s++;
// 	}
// 	return count;
// }

// // Main parse_line implementation
// char **parse_line(char *line)
// {
// 	if (line == NULL || *line == '\0')
// 		return (NULL);
// 	int argc = count_args(line);
// 	char **argv = malloc((argc + 1) * sizeof(char *));
// 	if (!argv) return NULL;
// 	int i = 0;
// 	const char *s = line;
// 	while (*s && i < argc)
// 	{
// 		skip_spaces(&s);
// 		if (!*s) break;
// 		const char *start = s;
// 		char quote = 0;
// 		size_t len = 0;
// 		if (*s == '\'' || *s == '"')
// 		{
// 			quote = *s;
// 			s++;
// 			start = s;
// 			while (*s && *s != quote)
// 				s++;
// 			len = s - start;
// 			argv[i] = substr_dup(start, len);
// 			if (*s == quote)
// 				s++;
// 		}
// 		else
// 		{
// 			while (s[len] && s[len] != ' ' && s[len] != '\t' && s[len] != '\'' && s[len] != '"')
// 				len++;
// 			   if (s[len] == '\'' || s[len] == '"')
// 			   {
// 				   // handle mixed quoted/unquoted
// 				   size_t pre = len;
// 				   quote = s[len];
// 				   len++;
// 				   const char *qstart = s + len;
// 				   while (s[len] && s[len] != quote)
// 					   len++;
// 				   size_t quoted_len = (s[len] == quote) ? (s + len - qstart) : 0;
// 				   argv[i] = substr_dup(start, pre);
// 				   char *quoted = substr_dup(qstart, quoted_len);
// 				   char *joined = malloc(pre + quoted_len + 1);
// 				   if (argv[i] && quoted && joined)
// 				   {
// 					   for (size_t j = 0; j < pre; j++) joined[j] = argv[i][j];
// 					   for (size_t j = 0; j < quoted_len; j++) joined[pre + j] = quoted[j];
// 					   joined[pre + quoted_len] = '\0';
// 					   free(argv[i]);
// 					   free(quoted);
// 					   argv[i] = joined;
// 				   }
// 				   if (s[len] == quote)
// 					   len++;
// 				   s += len;
// 				   i++;
// 				   continue;
// 			   }
// 			argv[i] = substr_dup(start, len);
// 			s += len;
// 		}
// 		i++;
// 	}
// 	argv[i] = NULL;
// 	return argv;
// }



#include "../includes/minishell.h"

// // Skip whitespaces
// static void skip_spaces(const char **s)
// {
// 	while (**s == ' ' || **s == '\t')
// 		(*s)++;
// }

// // Duplicate from start with len
// static char *substr_dup(const char *start, size_t len)
// {
// 	char *res = malloc(len + 1);
// 	if (!res)
// 		return NULL;
// 	for (size_t i = 0; i < len; i++)
// 		res[i] = start[i];
// 	res[len] = '\0';
// 	return res;
// }

// // Extract quoted string including the quote characters
// static char *extract_quoted_token(const char **s)
// {
// 	char quote = **s;
// 	const char *start = *s;
// 	(*s)++; // Skip opening quote
// 	while (**s && **s != quote)
// 		(*s)++;
// 	if (**s == quote)
// 		(*s)++; // Skip closing quote
// 	return substr_dup(start, *s - start);
// }

// // Extract unquoted token (until space or quote)
// static char *extract_unquoted_token(const char **s)
// {
// 	const char *start = *s;
// 	while (**s && **s != ' ' && **s != '\t' && **s != '\'' && **s != '"')
// 		(*s)++;
// 	return substr_dup(start, *s - start);
// }

// // Count number of tokens
// static int count_tokens(const char *s)
// {
// 	int count = 0;
// 	while (*s)
// 	{
// 		skip_spaces(&s);
// 		if (*s == '\0')
// 			break;
// 		if (*s == '\'' || *s == '"')
// 		{
// 			char quote = *s++;
// 			while (*s && *s != quote)
// 				s++;
// 			if (*s == quote)
// 				s++;
// 		}
// 		else
// 		{
// 			while (*s && *s != ' ' && *s != '\t' && *s != '\'' && *s != '"')
// 				s++;
// 		}
// 		count++;
// 	}
// 	return count;
// }

// // Main parser function
// char **parse_line(char *line)
// {
// 	if (!line || *line == '\0')
// 		return NULL;

// 	int argc = count_tokens(line);
// 	char **argv = malloc(sizeof(char *) * (argc + 1));
// 	if (!argv)
// 		return NULL;

// 	const char *s = line;
// 	int i = 0;
// 	while (*s && i < argc)
// 	{
// 		skip_spaces(&s);
// 		if (*s == '\0')
// 			break;
// 		if (*s == '\'' || *s == '"')
// 			argv[i++] = extract_quoted_token(&s);
// 		else
// 			argv[i++] = extract_unquoted_token(&s);
// 	}
// 	argv[i] = NULL;
// 	return argv;
// }

// Skip whitespaces
static void skip_spaces(const char **s)
{
    while (**s == ' ' || **s == '\t')
        (*s)++;
}

// Duplicate substring helper
static char *substr_dup(const char *start, size_t len)
{
    char *res = malloc(len + 1);
    if (!res)
        return NULL;
    for (size_t i = 0; i < len; i++)
        res[i] = start[i];
    res[len] = '\0';
    return res;
}

// Count tokens for allocation
// static int count_tokens(const char *s)
// {
//     int count = 0;
//     while (*s)
//     {
//         skip_spaces(&s);
//         if (*s == '\0')
//             break;

//         if (*s == '\'' || *s == '"')
//         {
//             char quote = *s++;
//             while (*s && *s != quote)
//                 s++;
//             if (*s == quote)
//                 s++;
//         }
//         else
//         {
//             while (*s && *s != ' ' && *s != '\t' && *s != '\'' && *s != '"')
//                 s++;
//         }
//         count++;
//     }
// 	printf("%d\n", count); // DEBUG
//     return count;
// }


static int count_tokens(const char *s)
{
    int count = 0;

    while (*s)
    {
        skip_spaces(&s);
        if (*s == '\0')
            break;

        // Start of token
        count++;

        // Move until space or end of token
        while (*s && *s != ' ' && *s != '\t')
        {
            if (*s == '\'' || *s == '"')
            {
                char quote = *s++;
                while (*s && *s != quote)
                    s++;
                if (*s == quote)
                    s++;
            }
            else if (*s == '=')
            {
                s++;
                // If next char is quote, consume quoted string as part of token
                if (*s == '\'' || *s == '"')
                {
                    char quote = *s++;
                    while (*s && *s != quote)
                        s++;
                    if (*s == quote)
                        s++;
                }
            }
            else
                s++;
        }
    }
	printf("%d\n", count); // DEBUG
    return count;
}


static t_token *create_token(const char *start, size_t len, t_quote_type quote)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->value = substr_dup(start, len);
    if (!token->value)
    {
        free(token);
        return NULL;
    }
    token->quote = quote;
    return token;
}

// Main parser function
t_token **parse_line(char *line)
{
    if (!line || *line == '\0')
        return NULL;

    int argc = count_tokens(line);
    t_token **tokens = malloc(sizeof(t_token *) * (argc + 1));
    if (!tokens)
        return NULL;

    const char *s = line;
    int i = 0;
    while (*s && i < argc)
    {
        skip_spaces(&s);
        if (*s == '\0')
            break;

        if (*s == '\'')
        {
            const char *start = ++s;
            while (*s && *s != '\'')
                s++;
            size_t len = s - start;
            tokens[i++] = create_token(start, len, SINGLE_QUOTE);
            if (*s == '\'')
                s++;
        }
        else if (*s == '"')
        {
            const char *start = ++s;
            while (*s && *s != '"')
                s++;
            size_t len = s - start;
            tokens[i++] = create_token(start, len, DOUBLE_QUOTE);
            if (*s == '"')
                s++;
        }
        else
        {
            const char *start = s;
            while (*s && *s != ' ' && *s != '\t' && *s != '\'' && *s != '"')
                s++;
            size_t len = s - start;
            tokens[i++] = create_token(start, len, NO_QUOTE);
        }
    }
    tokens[i] = NULL;
	print_tokens(tokens); // DEBUG: Print tokens for verification
	
    return tokens;
}
