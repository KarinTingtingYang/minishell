/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: makhudon <makhudon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:51:13 by tiyang            #+#    #+#             */
/*   Updated: 2025/09/03 11:26:39 by makhudon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H	

/** @brief Struct to represent an environment variable.
 *
 * This struct holds the key and value of an environment variable,
 * along with a pointer to the next variable in the linked list.
 */
typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}	t_env_var;

void		free_env(t_env_var *env);
t_env_var	*init_env(char **environ);
char		**env_list_to_array(t_env_var *env);
t_env_var	*duplicate_env_list(t_env_var *env_list);
void		add_env_var(const char *key, const char *value,
				t_env_var *env_list);
t_env_var	*find_env_var(const char *key, t_env_var *env_list);

#endif