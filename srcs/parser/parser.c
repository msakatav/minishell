/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/17 13:26:40 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (tokens->type >= TOKEN_REDIR_IN && \
				tokens->type <= TOKEN_REDIR_HEREDOC)
		{
			tokens = tokens->next;
			if (!tokens)
				break ;
		}
		tokens = tokens->next;
	}
	return (count);
}

static char	**parse_args(t_token **tokens, int **quote_types)
{
	char	**args;
	int		*quotes;
	int		i;
	int		count;

	count = count_args(*tokens);
	args = malloc(sizeof(char *) * (count + 1));
	quotes = malloc(sizeof(int) * (count + 1));
	if (!args || !quotes)
		return (NULL);
	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			args[i] = ft_strdup((*tokens)->value);
			quotes[i] = (*tokens)->quote_type;
			i++;
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type >= TOKEN_REDIR_IN && \
				(*tokens)->type <= TOKEN_REDIR_HEREDOC)
		{
			*tokens = (*tokens)->next;
			if (*tokens)
				*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	args[i] = NULL;
	quotes[i] = 0;
	*quote_types = quotes;
	return (args);
}

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
