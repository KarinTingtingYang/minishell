/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.h                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tiyang <tiyang@student.42.fr>                +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/31 11:51:13 by tiyang        #+#    #+#                 */
/*   Updated: 2025/07/31 11:53:52 by tiyang        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H	

typedef struct s_env_var // Structure to represent an environment variable
{
	char            *key;       // Variable name (key)
	char            *value;     // Variable value
	struct s_env_var *next; 	// Pointer to the next environment variable in the linked list
}   t_env_var;

char *get_env_value(t_env_var *env, char *key);
char **env_list_to_array(t_env_var *env);
t_env_var	*find_env_var(const char *key, t_env_var *env_list);
void		add_env_var(const char *key, const char *value, t_env_var *env_list);
int env_list_len(t_env_var *env);
t_env_var	*duplicate_env_list(t_env_var *env_list);

#endif