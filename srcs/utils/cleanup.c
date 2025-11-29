/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 14:09:32 by msakata          ###   ########TOKYO.jp  */
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

static void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp_redir;

	while (redirs)
	{
		tmp_redir = redirs;
		redirs = redirs->next;
		if (tmp_redir->type == TOKEN_REDIR_HEREDOC)
		{
			if (tmp_redir->file
				&& ft_strncmp(tmp_redir->file, "/tmp/.heredoc_", 14) == 0)
				unlink(tmp_redir->file);
		}
		if (tmp_redir->file)
			free(tmp_redir->file);
		free(tmp_redir);
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;

	while (cmds)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		if (tmp_cmd->args)
			ft_free_split(tmp_cmd->args);
		if (tmp_cmd->quote_types)
			free(tmp_cmd->quote_types);
		if (tmp_cmd->redirs)
			free_redirs(tmp_cmd->redirs);
		free(tmp_cmd);
	}
}
