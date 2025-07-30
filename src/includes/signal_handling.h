/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 14:31:47 by tiyang            #+#    #+#             */
/*   Updated: 2025/07/30 11:45:36 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_HANDLING_H
# define SIGNAL_HANDLING_H

# include <signal.h>    // For signal handling
# include <stdint.h>    // For sig_atomic_t
# include <sys/types.h> // For pid_t

// Function prototypes
void setup_signal_handlers(void);
void reset_child_signal_handlers(void);
int wait_for_child_and_handle_status(pid_t pid);
void handle_parent_sigint(int signum);
void print_signal_message(int status);

#endif