/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:31:59 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_separator(char c)
{
	return (ft_isspace(c) || c == '|' || c == '<' || c == '>');
}

static char	*extract_part(char *input, int *i, int *quote_char)
{
	if (input[*i] == '\'')
	{
		*quote_char = '\'';
		return (handle_quotes(input, i, '\''));
	}
	if (input[*i] == '"')
	{
		*quote_char = '"';
		return (handle_quotes(input, i, '"'));
	}
	*quote_char = 0;
	return (handle_word(input, i));
}

static void	append_part(char **result, char *part)
{
	char	*tmp;

	tmp = *result;
	*result = ft_strjoin(*result, part);
	free(tmp);
	free(part);
}

static char	*process_part(char *part, int quote, t_data *data)
{
	char	*tmp;

	if (quote != '\'')
	{
		tmp = expand_variables(part, data);
		free(part);
		return (tmp);
	}
	return (part);
}

static char	*build_combined_word(char *input, int *i, int *quote_type, t_data *data)
{
	char	*result;
	char	*part;
	int		has_unquoted;
	int		cur_quote;

	result = ft_strdup("");
	has_unquoted = 0;
	*quote_type = 0;
	while (input[*i] && !is_separator(input[*i]))
	{
		part = extract_part(input, i, &cur_quote);
		part = process_part(part, cur_quote, data);
		if (cur_quote == 0)
			has_unquoted = 1;
		else if (*quote_type == 0)
			*quote_type = cur_quote;
		append_part(&result, part);
	}
	if (has_unquoted)
		*quote_type = 0;
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
