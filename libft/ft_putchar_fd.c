/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 13:27:00 by makhudon          #+#    #+#             */
/*   Updated: 2025/05/04 13:11:46 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// What ft_putchar_fd must do
// 	•	write one character c to the file descriptor fd
//      (e.g. 1 for stdout, 2 for stderr, or a file)
// 	•	use the write() system call
// 	•	No return value

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
