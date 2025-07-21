/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:29:03 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:05:43 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What ft_itoa must do:
// convert an int into a string (char *) representation.

// The ft_itoa function converts an integer to
// a dynamically-allocated C-string (null-terminated).
// It must handle all integers (including INT_MIN),
// allocate memory with malloc, and produce the string
// representation of the number. 

static char	*write_digits(char *result, long int number, long int index)
{
	while (number > 0)
	{
		result[index] = '0' + (number % 10);
		number = number / 10;
		index--;
	}
	return (result);
}

static long int	count_chars(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*result;
	long int	number;
	long int	index;

	number = n;
	index = count_chars(n);
	result = malloc((index + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	result[index] = '\0';
	index--;
	if (number == 0)
		result[0] = '0';
	if (number < 0)
	{
		result[0] = '-';
		number = -number;
	}
	result = write_digits(result, number, index);
	return (result);
}
