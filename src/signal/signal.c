/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/21 12:41:05 by tiyang        #+#    #+#                 */
/*   Updated: 2025/08/05 09:15:42 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"
/**
 * @brief Sets up signal handlers for the minishell (parent process).
 *
 * Configures SIGINT (Ctrl+C) to use `handle_parent_sigint` and
 * SIGQUIT (Ctrl+\) to be ignored.
 */
void setup_signal_handlers(void) { //
    struct sigaction sa_int;
    struct sigaction sa_quit;

	// OPTIONAL: Tell readline to not handle signals internally.
    // This allows our custom signal handlers to take full control
    // and prevents readline from echoing '^C' or interfering.
    // rl_catch_signals = 0; // <--- This is the crucial line
	// without setting the variable to 0, the '^C' is echoed in terminal
	
	// rl_catch_signals = 0; // Disable readline's own signal handlers!
    // --- Configure SIGINT handler ---
    sa_int.sa_handler = handle_parent_sigint; // Set our custom handler
    sigemptyset(&sa_int.sa_mask);           // Clear the mask of signals to be blocked during handler execution
    sa_int.sa_flags = SA_RESTART;           // Restart functions interrupted by the signal (e.g., readline)

    if (sigaction(SIGINT, &sa_int, NULL) == -1) { //
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    // --- Configure SIGQUIT handler ---
    sa_quit.sa_handler = SIG_IGN;           // Ignore SIGQUIT
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;                   // No special flags needed for ignoring

    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) { //
        perror("sigaction SIGQUIT");
        exit(EXIT_FAILURE);
    }
    // No need to print "Signal handlers set up..." here.
}

/**
 * @brief Resets signal handlers to default behavior for child processes.
 *
 * This function should be called in the child process *after* fork() and
 * *before* execve(). This ensures that the executed command (e.g., `ls`)
 * handles signals with its default behavior, allowing SIGINT to terminate it.
 */
void reset_child_signal_handlers(void) { //
    struct sigaction sa_int_dfl;
    struct sigaction sa_quit_dfl;

    // --- Reset SIGINT to default behavior ---
    sa_int_dfl.sa_handler = SIG_DFL; // Default signal handling
    sigemptyset(&sa_int_dfl.sa_mask);
    sa_int_dfl.sa_flags = 0;

    if (sigaction(SIGINT, &sa_int_dfl, NULL) == -1) { //
        perror("sigaction SIGINT child");
        exit(EXIT_FAILURE); // Child should exit on error
    }

    // --- Reset SIGQUIT to default behavior ---
    sa_quit_dfl.sa_handler = SIG_DFL; // Default signal handling
    sigemptyset(&sa_quit_dfl.sa_mask);
    sa_quit_dfl.sa_flags = 0;

    if (sigaction(SIGQUIT, &sa_quit_dfl, NULL) == -1) { //
        perror("sigaction SIGQUIT child");
        exit(EXIT_FAILURE); // Child should exit on error
    }
}

// helper function to get wpid
static int	wait_for_child(pid_t pid, int *status)
{
    pid_t wpid;
	wpid = waitpid(pid, status, 0); // Call waitpid once
    while (wpid == -1 && errno == EINTR) {
        // If waitpid returns -1 and errno is EINTR, it means it was interrupted
        // by a signal. We simply continue waiting.
        wpid = waitpid(pid, status, 0); // Call waitpid again inside the loop
    }
    return wpid;
}

static int	get_exit_status(int status)
{
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return 0;
}

/**
 * @brief Waits for a child process to finish and handles its exit status.
 *
 * This function waits for the specified child process to terminate and
 * prints a message if it was terminated by a signal. It also returns the
 * exit status of the child process.
 *
 * @param pid The process ID of the child to wait for.
 * @return The exit status of the child process.
 */
int wait_for_child_and_handle_status(pid_t pid)
{
    int status;
    pid_t wpid;

    // g_child_running = 1;
    wpid = wait_for_child(pid, &status);
    // g_child_running = 0;
	
	// The g_child_running flag is no longer needed here. The parent's
    // main loop is blocked by waitpid, not readline, so the main
    // loop's signal handling logic won't run anyway.
    if (wpid != -1 && WIFSIGNALED(status))
        print_signal_message(status);
    else if (wpid == -1)
    {
        perror("waitpid");
        return -1;
    }
    return get_exit_status(status);
}