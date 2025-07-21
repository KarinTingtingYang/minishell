/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signal.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/21 12:41:05 by tiyang        #+#    #+#                 */
/*   Updated: 2025/07/21 14:11:30 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../src/includes/minishell.h"

/**
 * @brief Signal handler for SIGINT (Ctrl+C) in the parent process.
 *
 * This handler is designed to behave differently based on whether a child process
 * is currently executing.
 * - If no child is running (g_child_running == 0), it prints a new prompt.
 * - If a child is running (g_child_running == 1), it does nothing. This allows
 * the SIGINT to terminate the child, and the parent's waitpid will be
 * interrupted, leading it back to the prompt after the child exits.
 *
 * @param signum The signal number (expected to be SIGINT).
 */
void handle_parent_sigint(int signum)
{ //
    (void)signum; // Cast to void to suppress unused parameter warning

    // If no child is running, print a new prompt on SIGINT
    if (g_child_running == 0) { //
        ft_putstr_fd("\n", STDOUT_FILENO); // Move to a new line
        rl_on_new_line(); // Tell readline that we're on a new line
        rl_replace_line("", 0); // Clear the current input buffer
        rl_redisplay(); // Redraw the prompt
    }
    // If a child is running, do nothing.
    // The child will receive SIGINT and terminate, and the parent's waitpid
    // will be interrupted, returning control to the main loop.
}

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

/**
 * @brief Waits for a child process to terminate and handles its exit status,
 * including signal-related messages like "Quit (core dumped)".
 *
 * This function is called by the parent process after forking a child.
 * It manages the g_child_running flag and ensures waitpid is robust against EINTR.
 *
 * @param pid The process ID of the child to wait for.
 * @return The exit status of the child process, or -1 if waitpid encounters an error.
 * Returns 128 + signal_number if terminated by a signal.
 */
int wait_for_child_and_handle_status(pid_t pid)
{
    int status;
    pid_t wpid;

    // Set global flag to indicate a child process is running.
    // This tells the parent's SIGINT handler to not print a new prompt.
    g_child_running = 1;

    // Wait for the child process to terminate.
    // The do-while loop handles cases where waitpid is interrupted by a signal (EINTR),
    // ensuring the parent continues to wait until the child actually exits.
    wpid = waitpid(pid, &status, 0); // Call waitpid once
    while (wpid == -1 && errno == EINTR) {
        // If waitpid returns -1 and errno is EINTR, it means it was interrupted
        // by a signal. We simply continue waiting.
        wpid = waitpid(pid, &status, 0); // Call waitpid again inside the loop
    }

    // After the child has terminated (or waitpid encountered a non-EINTR error),
    // reset the global flag.
    g_child_running = 0;

    // Check if the child was terminated by a signal
    if (wpid != -1 && WIFSIGNALED(status)) {
        if (WTERMSIG(status) == SIGQUIT) {
            ft_putstr_fd("Quit", STDOUT_FILENO); // Print "Quit"
            if (WCOREDUMP(status)) {
                ft_putstr_fd(" (core dumped)", STDOUT_FILENO); // Print "(core dumped)" if applicable
            }
            ft_putstr_fd("\n", STDOUT_FILENO); // Always end with a newline
        } else if (WTERMSIG(status) == SIGINT) {
            // For SIGINT, just print a newline for clean prompt redraw
            // The ^C echo is handled by the terminal/readline by default (if rl_catch_signals is not 0)
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
    }
    // Handle other waitpid errors (non-EINTR)
    else if (wpid == -1) {
        perror("waitpid");
        return (-1); // Indicate an error in waiting
    }

    // Return appropriate exit status based on how the child exited
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status)); // Shell convention for signal termination
    return 0; // Should ideally be covered by WIFEXITED/WIFSIGNALED, but a safe default
}
