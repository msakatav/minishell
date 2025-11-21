/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/20 18:25:31 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_quotes(char *input, int *i, char quote_char)
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

static char	*handle_word(char *input, int *i)
{
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && \
		input[*i] != '|' && input[*i] != '<' && input[*i] != '>' && \
		input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static void	handle_redirect(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			add_token(tokens, new_token(TOKEN_REDIR_HEREDOC, ft_strdup("<<")));
			(*i) += 2;
		}
		else
		{
			add_token(tokens, new_token(TOKEN_REDIR_IN, ft_strdup("<")));
			(*i)++;
		}
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			add_token(tokens, new_token(TOKEN_REDIR_APPEND, ft_strdup(">>")));
			(*i) += 2;
		}
		else
		{
			add_token(tokens, new_token(TOKEN_REDIR_OUT, ft_strdup(">")));
			(*i)++;
		}
	}
}

static char	*build_combined_word(char *input, int *i, int *quote_type, t_data *data)
{
	char	*result;
	char	*part;
	char	*expanded;
	char	*tmp;
	int		has_unquoted;
	int		first_quote;

	result = ft_strdup("");
	has_unquoted = 0;
	first_quote = 0;
	while (input[*i] && !ft_isspace(input[*i]) && \
		input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'')
		{
			part = handle_quotes(input, i, '\'');
			if (!first_quote)
				first_quote = '\'';
		}
		else if (input[*i] == '"')
		{
			part = handle_quotes(input, i, '"');
			expanded = expand_variables(part, data);
			free(part);
			part = expanded;
			if (!first_quote)
				first_quote = '"';
		}
		else
		{
			part = handle_word(input, i);
			expanded = expand_variables(part, data);
			free(part);
			part = expanded;
			has_unquoted = 1;
		}
		tmp = result;
		result = ft_strjoin(result, part);
		free(tmp);
		free(part);
	}
	if (has_unquoted)
		*quote_type = 0;
	else
		*quote_type = first_quote;
	return (result);
}

static void	process_token(char *input, int *i, t_token **tokens, t_data *data)
{
	char	*value;
	t_token	*token;
	int		quote_type;

	if (input[*i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, ft_strdup("|")));
		(*i)++;
	}
	else if (input[*i] == '<' || input[*i] == '>')
		handle_redirect(input, i, tokens);
	else
	{
		value = build_combined_word(input, i, &quote_type, data);
		if (value && *value)
		{
			token = new_token(TOKEN_WORD, value);
			token->quote_type = quote_type;
			add_token(tokens, token);
		}
		else
			free(value);
	}
}

t_token	*lexer(char *input, t_data *data)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && ft_isspace(input[i]))
			i++;
		if (input[i])
			process_token(input, &i, &tokens, data);
	}
	return (tokens);
}
