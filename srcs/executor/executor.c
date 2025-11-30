/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 12:52:43 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_heredoc_redir(t_redir *redir)
{
	char	*filename;

	if (create_heredoc(redir->file, &filename) < 0)
		return (-1);
	free(redir->file);
	redir->file = filename;
	return (0);
}

static int	check_heredocs(t_cmd *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_REDIR_HEREDOC)
			{
				if (process_heredoc_redir(redir) < 0)
					return (-1);
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (0);
}

void	executor(t_cmd *cmds, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!cmds)
		return ;
	if (check_heredocs(cmds) < 0)
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
