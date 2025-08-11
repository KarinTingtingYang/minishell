/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/07 12:23:10 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Skips leading spaces in a string.
 * 
 * This function advances the pointer to the first non-space character in the string.
 * It handles spaces and tabs, but does not use the isspace function.
 * @param s Pointer to the string to process.
 */
static void skip_spaces(const char **s)
{
	while (**s && (**s == ' ' || **s == '\t'))
        (*s)++;
}

/**
 * @brief Appends a character to a dynamically allocated string.
 * 
 * This function appends a single character to a string, reallocating memory
 * as necessary. It returns the new string or NULL on failure.
 * @param str The original string to append to.
 * @param c The character to append.
 * @return A new string with the character appended, or NULL on failure.
 */
static char *substr_dup(const char *start, size_t len)
{
    char *result = malloc(len + 1);
    if (result == NULL)
        return NULL;
    size_t i = 0;
    while (i < len)
    {
        result[i] = start[i];
        i++;
    }
    result[len] = '\0';
    return result;
}

/**
 * @brief Counts the number of tokens in a string.
 * 
 * This function counts the number of tokens in a string, where tokens are
 * separated by spaces. It also handles quoted tokens correctly.
 * @param s The input string to count tokens in.
 * @return The number of tokens found in the string.
 */
// REDIRECTION DEBUGGING: need to be able to handle redirection operators
static int count_tokens(const char *s)
{
    int count = 0;
	char quote = 0;

    while (*s != '\0')
    {
        skip_spaces(&s);
        if (*s == '\0')
            break;
		// ============ REDIRECTION DEBUGGING
		// If we see a redirection operator, it's a token.
        if (!quote && (*s == '<' || *s == '>'))
        {
            count++;
            // Handle double operators like >> or <<
            if ((*s == '>' && *(s + 1) == '>') || (*s == '<' && *(s + 1) == '<'))
                s++;
            s++;
            continue;
        }
		// ================ END REDIRECTION DEBUGGING
        count++; // Found a token
        // while (*s != '\0' && ((*s != ' ' && *s != '\t') || quote != 0))
		// {
		// 	if (quote == 0 && (*s == '\'' || *s == '"'))
		// 		quote = *s; // Start of quoted token
		// 	else if (*s == quote)
		// 		quote = 0; // End of quoted token
		// 	s++;
		// }

		// CLEANED UP VERSION: loop logic more clear
		while (*s)
        {
            if (*s == quote)
                quote = 0; // Exiting quote mode
            else if (quote == 0 && (*s == '\'' || *s == '"'))
                quote = *s; // Entering quote mode
            else if (quote == 0 && (*s == ' ' || *s == '\t' || *s == '<' || *s == '>'))
                break; // End of token if we hit a space outside of quotes
            s++;
        }
    }
    return count;
}

/**
 * @brief Creates a new t_token structure with the given value and quote type.
 * 
 * This function allocates memory for a new t_token structure, sets its value
 * and quote type, and returns a pointer to the newly created token.
 * @param value The string value of the token.
 * @param quote_type The type of quote enclosing the token (if any).
 * @return A pointer to the newly created t_token structure, or NULL on failure.
 */
static t_token *create_token(char *value, t_quote_type quote_type)
{
    t_token *token = malloc(sizeof(t_token));
    if (token == NULL)
        return NULL;
    token->value = value;
    token->quote = quote_type; // Set the quote type
    return token;
}

/**
 * @brief Parses a line of input into an array of tokens.
 * 
 * This function takes a line of input, splits it into tokens based on spaces,
 * and returns an array of t_token structures. Each token contains its value
 * and the type of quote it was enclosed in (if any).
 * @param line The input line to parse.
 * @return An array of t_token pointers, NULL-terminated, or NULL on failure.
 */
// REDIRECTION DEBUGGING: need to be able to handle redirection operators
t_token **parse_line(char *line)
{
	int token_count;
	
	if (line == NULL || *line == '\0')
		return NULL;
	token_count = count_tokens(line);
	t_token **tokens = malloc(sizeof(t_token *) * (token_count + 1));
	if (tokens == NULL)
		return NULL;
    const char *s = line; // Pointer to traverse the input line
    int i = 0;
	char *current_token_val; // To store the current token value
    char quote_char; // To track quote type
    while (*s != '\0' && i < token_count)
    {
        skip_spaces(&s);
        if (*s == '\0')
            break ; // End of line
		const char *start = s; // Start of the current token
        quote_char = 0; // Reset quote character for each token

		// ADDED FOR QUOTE HANDLING & REDIRECTION DEBUGGING
		t_quote_type token_quote_type = NO_QUOTE;
		// Check for redirection operator as a token
        if (*s == '<' || *s == '>')
        {
            if ((*s == '>' && *(s + 1) == '>') || (*s == '<' && *(s + 1) == '<'))
                s += 2;
            else
                s++;
        }
		else // It's a word or quoted string
		{
			if (*s == '\'')
            	token_quote_type = SINGLE_QUOTE;
       		else if (*s == '"')
            	token_quote_type = DOUBLE_QUOTE;

        // while (*s != '\0' && ((*s != ' ' && *s != '\t') || quote_char))
		// {
		// 	if (quote_char == 0 && (*s == '\'' || *s == '"')) // Start of quoted token
		// 		quote_char = *s; // Set quote character
		// 	else if (*s == quote_char) // End of quoted token
		// 		quote_char = 0; // Reset quote character
        //     s++;
        // }
		// CLEANED UP VERSION: loop logic more clear
			while (*s)
        	{
            	if (*s == quote_char)
                	quote_char = 0;
            	else if (quote_char == 0 && (*s == '\'' || *s == '"'))
                	quote_char = *s;
            	else if (quote_char == 0 && (*s == ' ' || *s == '\t' || *s == '<' || *s == '>'))
                	break;
            	s++;
        	}
		}
        current_token_val = substr_dup(start, s - start);
        tokens[i++] = create_token(current_token_val, token_quote_type); // Assuming no quotes for now
    }
    tokens[i] = NULL;
	print_tokens(tokens); // DEBUG: Print the parsed tokens
    return tokens;
}
