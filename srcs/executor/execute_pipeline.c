/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 06:16:22 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_external_in_child(t_cmd *cmd, t_data *data)
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

static void	execute_child(t_cmd *cmd, t_data *data, int *prev, int *pfd)
{
	if (prev[0] != -1)
	{
		dup2(prev[0], STDIN_FILENO);
		close(prev[0]);
		close(prev[1]);
	}
	if (cmd->next)
	{
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
	}
	if (setup_redirections(cmd->redirs, data) < 0)
		exit(1);
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, data));
	else
		exec_external_in_child(cmd, data);
}

static pid_t	process_pipeline_cmd(t_cmd *cmd,
	t_data *data, int *prev, int *pfd)
{
	pid_t	pid;

	if (cmd->next && pipe(pfd) < 0)
	{
		print_error("pipe", strerror(errno));
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		execute_child(cmd, data, prev, pfd);
	else if (pid > 0)
		handle_parent_pipeline(prev, pfd, cmd);
	return (pid);
}

void	execute_pipeline(t_cmd *cmds, t_data *data)
{
	int		pipe_fd[2];
	int		prev_pipe[2];
	pid_t	last_pid;
	int		cmd_count;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd_count = count_cmds(cmds);
	last_pid = -1;
	while (cmds)
	{
		last_pid = process_pipeline_cmd(cmds, data, prev_pipe, pipe_fd);
		cmds = cmds->next;
	}
	if (prev_pipe[0] != -1)
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	wait_for_children(cmd_count, last_pid, data);
}
