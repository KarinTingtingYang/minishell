# minishell

# July 31 - tiyang
- fixed bug for export built-in (ft_strlcpy issue)
- added argument handling for export built-in
- added separate folder "env" and header file env.h for t_env_var *env_list related functions
- replaced qsort logic with bubble sort for export built-in as qsort is not allowed for this project
## To do
- replace sprintf in src/env/env_utils.c as it is not allowed for this project
- unify the function names and return values of built-ins (run_echo, builtin_cd, run_export, etc.)
- better annotation for t_env_var *env_list related functions
