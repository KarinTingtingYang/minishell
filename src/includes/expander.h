/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 12:14:24 by makhudon          #+#    #+#             */
/*   Updated: 2025/08/05 10:40:31 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

char *expand_variables(char *input, t_env_var *env_list, int last_exit_status);
// void expand_args(char **args, t_env_var *env_list, int last_exit_status);
char **expand_and_split_args(char **args, t_env_var *env_list, int last_exit_status);
char **ft_split_whitespace(char *s);

#endif
