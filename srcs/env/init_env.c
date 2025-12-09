/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 20:07:54 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	if (!node->key || !node->value)
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	return (node);
}

static void	add_env_node(t_env **env, t_env *new)
{
	t_env	*current;

	if (!new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = new;
}

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

t_env	*init_env(char **envp)
{
	t_env	*env;
	char	*equal;
	char	*key;
	char	*value;
	int		i;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			key = ft_substr(envp[i], 0, equal - envp[i]);
			value = ft_strdup(equal + 1);
			if (key && value)
				add_env_node(&env, new_env_node(key, value));
			free(key);
			free(value);
		}
		i++;
	}
	ensure_pwd(&env);
	update_shlvl(&env);
	ensure_underscore(&env);
	return (env);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
