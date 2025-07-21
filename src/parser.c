/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:05:27 by makhudon          #+#    #+#             */
/*   Updated: 2025/07/21 11:13:49 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

char **parse_line(char *line)
{
	if (line == NULL || *line == '\0')
		return (NULL);
	return (ft_split(line, ' '));
}
