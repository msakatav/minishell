/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/21 23:18:24 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_external_cmd(t_cmd *cmd, t_data *data)
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
	if (!envp)
		exit(1);
	execve(path, cmd->args, envp);
	print_error(cmd->args[0], strerror(errno));
	free(path);
	ft_free_split(envp);
	exit(126);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	if (is_builtin(cmd->args[0]))
	{
		if (setup_redirections(cmd->redirs, data) < 0)
		{
			data->exit_status = 1;
			return ;
		}
		data->exit_status = execute_builtin(cmd, data);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		if (setup_redirections(cmd->redirs, data) < 0)
			exit(1);
		execute_external_cmd(cmd, data);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			data->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			data->exit_status = 128 + WTERMSIG(status);
	}
	else
		print_error("fork", strerror(errno));
}
