/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 13:00:14 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

void	handle_parent_pipeline(int *prev_pipe, int *pipe_fd, t_cmd *cmd)
{
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next)
	{
		prev_pipe[0] = pipe_fd[0];
		prev_pipe[1] = pipe_fd[1];
	}
}

void	wait_for_children(int cmd_count, pid_t last_pid, t_data *data)
{
	int		status;
	pid_t	pid;
	int		signal_printed;

	signal_printed = 0;
	while (cmd_count-- > 0)
	{
		pid = wait(&status);
		if (WIFSIGNALED(status) && !signal_printed)
		{
			if (WTERMSIG(status) == SIGINT)
				write(1, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", 2);
			signal_printed = 1;
		}
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = 128 + WTERMSIG(status);
		}
	}
}
