/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 13:00:14 by msakata          ###   ########TOKYO.jp  */
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

static void	handle_child_process(t_cmd *cmd, t_data *data)
{
	if (setup_redirections(cmd->redirs, data) < 0)
		exit(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execute_external_cmd(cmd, data);
}

static void	handle_parent_process(pid_t pid, t_data *data)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		data->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", 2);
	}
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	pid_t	pid;

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
		handle_child_process(cmd, data);
	else if (pid > 0)
		handle_parent_process(pid, data);
	else
		print_error("fork", strerror(errno));
}
