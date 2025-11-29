/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 11:27:44 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor(t_cmd *cmds, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!cmds)
		return ;
	signal(SIGINT, SIG_IGN);
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (cmds->next)
		execute_pipeline(cmds, data);
	else
		execute_cmd(cmds, data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	setup_signals();
}
