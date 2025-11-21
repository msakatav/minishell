/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:20:27 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd(t_data *data, char *old_pwd)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		set_env_value(&data->env, "OLDPWD", old_pwd);
		set_env_value(&data->env, "PWD", cwd);
	}
}

static char	*get_cd_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(data->env, "HOME");
		if (!path)
			print_error("cd", "HOME not set");
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = get_env_value(data->env, "OLDPWD");
		if (!path)
			print_error("cd", "OLDPWD not set");
		else
			ft_putendl_fd(path, 1);
	}
	else
		path = args[1];
	return (path);
}

int	builtin_cd(char **args, t_data *data)
{
	char	*path;
	char	old_pwd[1024];

	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (1);
	if (args[1] && args[2])
	{
		print_error("cd", "too many arguments");
		return (1);
	}
	path = get_cd_path(args, data);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		print_error("cd", strerror(errno));
		return (1);
	}
	update_pwd(data, old_pwd);
	return (0);
}
