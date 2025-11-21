/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:31:59 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*parse_redirections(t_token **tokens)
{
	t_redir	*redirs;
	t_redir	*new;
	int		type;

	redirs = NULL;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type >= TOKEN_REDIR_IN && \
			(*tokens)->type <= TOKEN_REDIR_HEREDOC)
		{
			type = (*tokens)->type;
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_WORD)
			{
				new = new_redir(type, ft_strdup((*tokens)->value));
				add_redir(&redirs, new);
				*tokens = (*tokens)->next;
			}
		}
		else
			*tokens = (*tokens)->next;
	}
	return (redirs);
}

static t_cmd	*parse_command(t_token **tokens)
{
	t_cmd	*cmd;
	t_token	*start;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	start = *tokens;
	cmd->redirs = parse_redirections(&start);
	start = *tokens;
	cmd->args = parse_args(&start, &cmd->quote_types);
	*tokens = start;
	return (cmd);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cmd;

	cmds = NULL;
	while (tokens)
	{
		cmd = parse_command(&tokens);
		if (cmd)
			add_cmd(&cmds, cmd);
		if (tokens && tokens->type == TOKEN_PIPE)
			tokens = tokens->next;
	}
	return (cmds);
}
