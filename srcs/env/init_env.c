/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 21:58:36 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_shlvl(t_env **env)
{
	t_env	*curr;
	int		lvl;

	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "SHLVL") == 0)
		{
			lvl = ft_atoi(curr->value) + 1;
			free(curr->value);
			curr->value = ft_itoa(lvl);
			return ;
		}
		curr = curr->next;
	}
	add_env_node(env, new_env_node("SHLVL", "1"));
}

static void	ensure_pwd(t_env **env)
{
	t_env	*curr;
	char	*cwd;

	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "PWD") == 0)
			return ;
		curr = curr->next;
	}
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		add_env_node(env, new_env_node("PWD", cwd));
		free(cwd);
	}
}

static void	ensure_underscore(t_env **env)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "_") == 0)
			return ;
		curr = curr->next;
	}
	add_env_node(env, new_env_node("_", "/usr/bin/env"));
}

static void	process_env_entry(t_env **env, char *entry)
{
	char	*equal;
	char	*key;
	char	*value;

	equal = ft_strchr(entry, '=');
	if (equal)
	{
		key = ft_substr(entry, 0, equal - entry);
		value = ft_strdup(equal + 1);
		if (key && value)
			add_env_node(env, new_env_node(key, value));
		free(key);
		free(value);
	}
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		process_env_entry(&env, envp[i]);
		i++;
	}
	ensure_pwd(&env);
	update_shlvl(&env);
	ensure_underscore(&env);
	return (env);
}
