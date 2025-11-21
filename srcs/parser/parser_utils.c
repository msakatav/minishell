/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/21 23:18:24 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->quote_types = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_cmd(t_cmd **cmds, t_cmd *new)
{
	t_cmd	*current;

	if (!new)
		return ;
	if (!*cmds)
	{
		*cmds = new;
		return ;
	}
	current = *cmds;
	while (current->next)
		current = current->next;
	current->next = new;
}

t_redir	*new_redir(int type, char *file)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = file;
	redir->next = NULL;
	return (redir);
}

void	add_redir(t_redir **redirs, t_redir *new)
{
	t_redir	*current;

	if (!new)
		return ;
	if (!*redirs)
	{
		*redirs = new;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;

	while (cmds)
	{
		tmp_cmd = cmds;
		cmds = cmds->next;
		if (tmp_cmd->args)
			ft_free_split(tmp_cmd->args);
		if (tmp_cmd->quote_types)
			free(tmp_cmd->quote_types);
		while (tmp_cmd->redirs)
		{
			tmp_redir = tmp_cmd->redirs;
			tmp_cmd->redirs = tmp_cmd->redirs->next;
			if (tmp_redir->file)
				free(tmp_redir->file);
			free(tmp_redir);
		}
		free(tmp_cmd);
	}
}
