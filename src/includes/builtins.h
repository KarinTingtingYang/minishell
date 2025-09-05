/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 09:01:24 by makhudon          #+#    #+#             */
/*   Updated: 2025/09/05 14:03:29 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		run_pwd(void);
int		is_builtin(char *cmd);
void	run_env(t_env_var *env_list);
int		is_valid_identifier(const char *str);
int		perform_single_pass(t_env_var *start);
int		run_cd(char **args, t_env_var *env_list);
void	run_echo(char **args, t_env_var *env_list);
int		run_exit(char **args, int last_exit_status);
int		run_unset(t_env_var **env_list, char **args);
int		run_export(t_env_var *env_list, char **args);
int		ft_str_to_llong(const char *str, long long *out_val);
int		export_variable(const char *arg, t_env_var *env_list);
int		run_builtin(char **args, t_process_data *process_data);
int		apply_builtin_redirection(char *input_file, char *output_file,
			int output_mode);
int		ft_parse_digits_to_llong(const char *str,
										int sign, long long *out_val);
#endif