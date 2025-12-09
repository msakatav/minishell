/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 21:58:36 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	check_pipe_error(t_token *tokens, t_data *data)
{
	if (tokens->type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		data->exit_status = 258;
		return (1);
	}
	return (0);
}

static int	check_trailing_pipe(t_token **tokens, t_data *data)
{
	if (*tokens && (*tokens)->type == TOKEN_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens)
		{
			ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
			data->exit_status = 258;
			return (1);
		}
	}
	return (0);
}

static int	process_token_group(t_token **tokens, t_cmd **cmds, t_data *data)
{
	t_cmd	*cmd;

	if (check_pipe_error(*tokens, data))
		return (1);
	cmd = parse_command(tokens, data);
	if (!cmd)
		return (1);
	add_cmd(cmds, cmd);
	if (check_trailing_pipe(tokens, data))
		return (1);
	return (0);
}

t_cmd	*parser(t_token *tokens, t_data *data)
{
	t_cmd	*cmds;

	cmds = NULL;
	while (tokens)
	{
		if (process_token_group(&tokens, &cmds, data))
		{
			if (cmds)
				free_cmds(cmds);
			return (NULL);
		}
	}
	return (cmds);
}
