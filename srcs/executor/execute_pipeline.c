/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/20 18:57:48 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_cmd *cmds)
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

static void	execute_child(t_cmd *cmd, t_data *data, int *prev_pipe, int *pipe_fd)
{
	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (setup_redirections(cmd->redirs, data) < 0)
		exit(1);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, data));
	else
	{
		char	*path;
		char	**envp;

		path = find_executable(cmd->args[0], data->env);
		if (!path)
		{
			print_error(cmd->args[0], "command not found");
			exit(127);
		}
		if (access(path, X_OK) != 0)
		{
			print_error(path, "Permission denied");
			free(path);
			exit(126);
		}
		envp = env_to_array(data->env);
		execve(path, cmd->args, envp);
		exit(126);
	}
}

void	execute_pipeline(t_cmd *cmds, t_data *data)
{
	int		pipe_fd[2];
	int		prev_pipe[2];
	pid_t	pid;
	pid_t	last_pid;
	int		status;
	int		cmd_count;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd_count = count_cmds(cmds);
	last_pid = -1;
	while (cmds)
	{
		if (cmds->next && pipe(pipe_fd) < 0)
		{
			print_error("pipe", strerror(errno));
			return ;
		}
		pid = fork();
		if (pid == 0)
			execute_child(cmds, data, prev_pipe, pipe_fd);
		else if (pid > 0)
		{
			last_pid = pid;
			if (prev_pipe[0] != -1)
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (cmds->next)
			{
				prev_pipe[0] = pipe_fd[0];
				prev_pipe[1] = pipe_fd[1];
			}
		}
		cmds = cmds->next;
	}
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	while (cmd_count-- > 0)
	{
		pid = wait(&status);
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = 128 + WTERMSIG(status);
		}
	}
}
