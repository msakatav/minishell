/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:31:59 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_quotes(char *input, int *i, char quote_char)
{
	int		start;
	char	*value;

	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] == quote_char)
	{
		value = ft_substr(input, start + 1, *i - start - 1);
		(*i)++;
	}
	else
		value = ft_substr(input, start + 1, *i - start - 1);
	return (value);
}

char	*handle_word(char *input, int *i)
{
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && \
		input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && \
		input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static void	add_redir_token(t_token **tokens, int type, char *val, int *i)
{
	add_token(tokens, new_token(type, ft_strdup(val)));
	if (type == TOKEN_REDIR_HEREDOC || type == TOKEN_REDIR_APPEND)
		(*i) += 2;
	else
		(*i)++;
}

void	handle_redirect(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
			add_redir_token(tokens, TOKEN_REDIR_HEREDOC, "<<", i);
		else
			add_redir_token(tokens, TOKEN_REDIR_IN, "<", i);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
			add_redir_token(tokens, TOKEN_REDIR_APPEND, ">>", i);
		else
			add_redir_token(tokens, TOKEN_REDIR_OUT, ">", i);
	}
}
