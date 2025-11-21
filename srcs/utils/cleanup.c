/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/17 13:11:42 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_data(t_data *data)
{
	if (!data)
		return ;
	if (data->cmds)
		free_cmds(data->cmds);
	if (data->env)
		free_env(data->env);
	if (data->stdin_backup != -1)
		close(data->stdin_backup);
	if (data->stdout_backup != -1)
		close(data->stdout_backup);
}
