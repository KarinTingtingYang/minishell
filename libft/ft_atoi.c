/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:35:37 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:03:15 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What does ft_atoi do:
// • ft_atoi stands for ASCII to Integer.
// • it converts a string like " -42" into the number -42.

int	ft_atoi(const char *str)
{
	unsigned long	number;
	int				sign;

	number = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t'
		|| *str == '\n' || *str == '\v'
		|| *str == '\f' || *str == '\r')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		number = number * 10 + (*str - '0');
		if (sign == 1 && number > INT_MAX)
			return (INT_MAX);
		if (sign == -1 && number > (unsigned long)INT_MAX + 1)
			return (INT_MIN);
		str++;
	}
	return ((int)(number * sign));
}
