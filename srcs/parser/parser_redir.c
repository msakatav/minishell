/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:00:00 by msakata           #+#    #+#             */
/*   Updated: 2025/12/09 21:58:36 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redir_error(t_token *token, t_data *data, int *error)
{
	if (token && token->type == TOKEN_EMPTY_EXPANSION)
	{
		ft_putendl_fd("minishell: ambiguous redirect", 2);
		data->exit_status = 1;
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (token)
			ft_putstr_fd(token->value, 2);
		else
			ft_putstr_fd("newline", 2);
		ft_putendl_fd("'", 2);
		data->exit_status = 258;
	}
	*error = 1;
}

static int	process_redir(t_token **tokens, t_redir **redirs, t_data *data,
	int *error)
{
	int		type;
	t_redir	*new;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		new = new_redir(type, ft_strdup((*tokens)->value));
		add_redir(redirs, new);
		*tokens = (*tokens)->next;
	}
	else
	{
		handle_redir_error(*tokens, data, error);
		return (0);
	}
	return (1);
}

t_redir	*parse_redirections(t_token **tokens, t_data *data, int *error)
{
	t_redir	*redirs;

	redirs = NULL;
	*error = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type >= TOKEN_REDIR_IN
			&& (*tokens)->type <= TOKEN_REDIR_HEREDOC)
		{
			if (!process_redir(tokens, &redirs, data, error))
			{
				free_redirs(redirs);
				return (NULL);
			}
		}
		else
			*tokens = (*tokens)->next;
	}
	return (redirs);
}
