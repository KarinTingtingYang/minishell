# Minishell

As beautiful as a shell! This project is a custom implementation of a minimalistic bash-like shell, built for the Codam / 42 school curriculum. It parses, expands, and executes commands while handling environment variables, pipelines, redirections, and signals.

## 👥 Authors
* **tiyang** (karintingtingyang)
* **makhudon** (MariiaKhud)

## ✨ Features

Our minishell mimics the core behavior of bash, including:

### 1. Command Execution
* Searches and executes commands using the `PATH` variable or via absolute/relative paths.
* Supports running single commands and complex pipelines using the `|` operator.
* Handles both interactive (prompt-based) and non-interactive (reading from `stdin`) modes.

### 2. Built-in Commands
The following built-ins are implemented natively:
* `echo` with the `-n` option.
* `cd` with only a relative or absolute path.
* `pwd` with no options.
* `export` with no options (sorts and displays environment variables or adds new ones).
* `unset` with no options.
* `env` with no options or arguments.
* `exit` with an optional numeric exit status.

### 3. Redirections
Handles standard shell I/O redirections:
* `<` redirect input.
* `>` redirect output (truncate).
* `>>` redirect output (append).
* `<<` (heredoc) reads input until a delimiter is reached, fully supporting variable expansion unless the delimiter is quoted.

### 4. Parsing and Lexing
* **Quotes**: Handles single (`'`) and double (`"`) quotes. Single quotes prevent variable expansion, while double quotes allow it.
* **Variable Expansion**: Expands environment variables (e.g., `$USER`) and the special `$?` variable (which holds the exit status of the most recently executed foreground pipeline).

### 5. Signal Handling
* `ctrl-C` (SIGINT): Displays a new prompt on a new line.
* `ctrl-D` (EOF): Exits the shell.
* `ctrl-\` (SIGQUIT): Ignored in the parent process, but terminates child processes with a core dump message.

## 🧠 Architecture Flow

We have carefully documented the logic of our shell using Mermaid flowcharts (available in the markdown files). Here is a high-level overview of our architecture:

1. **Input**: Read via `readline` (interactive) or `get_next_line` (non-interactive).
2. **Lexing & Parsing**: The raw string is tokenized, checking for unclosed quotes and syntax errors.
3. **Expansion**: Variables (`$VAR`) are expanded based on quoting rules.
4. **Command Creation**: Clean arguments are separated from redirections to build a `t_command` structure.
5. **Execution**: 
   * **Single Commands**: Built-ins run in the parent process if there are no redirections; otherwise, the process forks.
   * **Pipelines**: We fork recursively, creating pipes (`pipe()`) and connecting `stdin`/`stdout` using `dup2()` before calling `execve()`.

## 🚀 Installation & Usage

### Prerequisites
* `gcc` or `cc`
* `make`
* `readline` library

### Building the Project
Run the following command in the root directory to compile the project:

make

This will compile the libft library and the minishell source files into the minishell executable.

### Running Minishell

./minishell

### Makefile Rules
make - Compiles the project.

make clean - Removes object files.

make fclean - Removes object files and the executable.

make re - Recompiles the entire project.

---
*Created for the Codam (42 Network) curriculum.*
