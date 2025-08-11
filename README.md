# minishell

# August 11  tiyang
- fixed quote handling & argument splitting issues in the parser and expander
- added redirection tokens to the parsing/tokenising logic to correctly handle redirection
- centralised tokenising/parsing logic to parse_line (now tokenize_input calls parse_line)
- fixed variable expansion issues when running pipeline commands
## TO DO
- check code redundancy in parser code

# August 6 - tiyang
- enhanced run_cd function to handle cd command with no argument (should return to home)
- enhanced error handling in exit built-in and fixed exit code propagation issues
- added printf debug features
- fixed quote handling issues in variable export (export TEST="test")
## TO DO
- need to further test new quote handling code in parser.c

# July 31 - tiyang
## To do
- unify the function names and return values of built-ins (run_echo, builtin_cd, run_export, etc.)
- better annotation for t_env_var *env_list related functions


