/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 20:52:27 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*parse_redirections(t_token **tokens, t_data *data, int *error)
{
	t_redir	*redirs;
	t_redir	*new;
	int		type;

	redirs = NULL;
	*error = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type >= TOKEN_REDIR_IN
			&& (*tokens)->type <= TOKEN_REDIR_HEREDOC)
		{
			type = (*tokens)->type;
			*tokens = (*tokens)->next;
			if (*tokens && (*tokens)->type == TOKEN_WORD)
			{
				new = new_redir(type, ft_strdup((*tokens)->value));
				add_redir(&redirs, new);
				*tokens = (*tokens)->next;
			}
			else if (*tokens && (*tokens)->type == TOKEN_EMPTY_EXPANSION)
			{
				ft_putendl_fd("minishell: ambiguous redirect", 2);
				data->exit_status = 1;
				*error = 1;
				free_redirs(redirs);
				return (NULL);
			}
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
				if (*tokens)
					ft_putstr_fd((*tokens)->value, 2);
				else
					ft_putstr_fd("newline", 2);
				ft_putendl_fd("'", 2);
				data->exit_status = 258;
				*error = 1;
				free_redirs(redirs);
				return (NULL);
			}
		}
		else
			*tokens = (*tokens)->next;
	}
	return (redirs);
}

static t_cmd	*parse_command(t_token **tokens, t_data *data)
{
	t_cmd	*cmd;
	t_token	*start;
	int		error;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	start = *tokens;
	cmd->redirs = parse_redirections(&start, data, &error);
	if (error)
	{
		free(cmd);
		return (NULL);
	}
	start = *tokens;
	cmd->args = parse_args(&start, &cmd->quote_types);
	*tokens = start;
	return (cmd);
}

t_cmd	*parser(t_token *tokens, t_data *data)
{
	t_cmd	*cmds;
	t_cmd	*cmd;

	cmds = NULL;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
			data->exit_status = 258;
			if (cmds)
				free_cmds(cmds);
			return (NULL);
		}
		cmd = parse_command(&tokens, data);
		if (!cmd)
		{
			if (cmds)
				free_cmds(cmds);
			return (NULL);
		}
		add_cmd(&cmds, cmd);
		if (tokens && tokens->type == TOKEN_PIPE)
		{
			tokens = tokens->next;
			if (!tokens)
			{
				ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
				data->exit_status = 258;
				free_cmds(cmds);
				return (NULL);
			}
		}
	}
	return (cmds);
}
