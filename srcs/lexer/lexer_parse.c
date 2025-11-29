/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 07:10:55 by msakata           #+#    #+#             */
/*   Updated: 2025/11/29 14:09:32 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	protect_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			str[i] = 1;
		else if (str[i] == '\t')
			str[i] = 2;
		else if (str[i] == '\n')
			str[i] = 3;
		else if (str[i] == '\v')
			str[i] = 4;
		else if (str[i] == '\f')
			str[i] = 5;
		else if (str[i] == '\r')
			str[i] = 6;
		i++;
	}
}

static char	*process_part(char *part, int quote, t_data *data)
{
	char	*tmp;

	if (quote != '\'')
	{
		tmp = expand_variables(part, data);
		free(part);
		if (quote == '"')
			protect_spaces(tmp);
		return (tmp);
	}
	protect_spaces(part);
	return (part);
}

char	*build_combined_word(char *input, int *i,
	int *quote_type, t_data *data)
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
