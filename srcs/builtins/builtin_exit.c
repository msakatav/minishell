/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 20:34:13 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
		i++;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i])
		return (0);
	return (1);
}

static int	check_overflow(char *str)
{
	unsigned long long	num;
	int					sign;
	int					i;

	num = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (num > 922337203685477580 || (num == 922337203685477580
				&& ((sign == 1 && str[i] - '0' > 7)
					|| (sign == -1 && str[i] - '0' > 8))))
			return (1);
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (0);
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_code;

	ft_putendl_fd("exit", 2);
	if (!args[1])
	{
		cleanup_data(data);
		rl_clear_history();
		exit(data->exit_status);
	}
	if (!is_numeric(args[1]) || check_overflow(args[1]))
	{
		print_error("exit", "numeric argument required");
		cleanup_data(data);
		rl_clear_history();
		exit(2);
	}
	if (args[2])
	{
		print_error("exit", "too many arguments");
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	cleanup_data(data);
	rl_clear_history();
	exit(exit_code % 256);
}
