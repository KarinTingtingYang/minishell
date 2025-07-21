/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:23:45 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 14:17:33 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

// What ft_putnbr_fd must do:
// 	•	convert an int (n) to characters
// 	•	write the characters to the given file descriptor fd
// 	•	use only the write() function
// 	•	handle negative numbers and INT_MIN

void	ft_putnbr_fd(int n, int fd)
{
	long	nb;

	nb = n;
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr_fd(nb / 10, fd);
	}
	ft_putchar_fd((nb % 10) + '0', fd);
}
