/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:31:47 by tiyang            #+#    #+#             */
/*   Updated: 2025/08/30 14:26:00 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLING_H
# define SIGNAL_HANDLING_H

int		signal_event_hook(void);
void	setup_signal_handlers(void);
void	handle_parent_sigint(int signum);
void	print_signal_message(int status);
void	reset_child_signal_handlers(void);
int		wait_for_child_and_handle_status(pid_t pid);

#endif