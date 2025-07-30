/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:46:49 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/30 09:34:03 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What ft_putstr_fd must do:
// • write the entire string s
// • to the file descriptor fd 
// • use only the write() function
// • Must not return anything

void	ft_putstr_fd(const char *s, int fd)
{
	int	i;

	if (s == NULL)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
}
