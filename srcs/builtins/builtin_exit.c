/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/20 18:48:19 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_code;

	ft_putendl_fd("exit", 2);
	if (!args[1])
	{
		cleanup_data(data);
		exit(data->exit_status);
	}
	if (!is_numeric(args[1]))
	{
		print_error("exit", "numeric argument required");
		cleanup_data(data);
		exit(2);
	}
	if (args[2])
	{
		print_error("exit", "too many arguments");
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	cleanup_data(data);
	exit(exit_code % 256);
}
