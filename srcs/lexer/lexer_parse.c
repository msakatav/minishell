/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 07:10:55 by msakata           #+#    #+#             */
/*   Updated: 2025/12/09 21:58:36 by msakata          ###   ########TOKYO.jp  */
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

static int	handle_word_part(char *input, int *i, char **result, t_data *data)
{
	char	*part;
	int		cur_quote;
	char	*tmp;

	part = extract_part(input, i, &cur_quote);
	if (!part)
		return (-1);
	part = process_part(part, cur_quote, data);
	tmp = *result;
	*result = ft_strjoin(*result, part);
	free(tmp);
	free(part);
	return (cur_quote);
}

char	*build_combined_word(char *input, int *i,
	int *quote_type, t_data *data)
{
	char	*result;
	int		has_unquoted;
	int		cur_quote;

	result = ft_strdup("");
	has_unquoted = 0;
	*quote_type = 0;
	while (input[*i] && !is_separator(input[*i]))
	{
		cur_quote = handle_word_part(input, i, &result, data);
		if (cur_quote == -1)
		{
			free(result);
			return (NULL);
		}
		if (cur_quote == 0)
			has_unquoted = 1;
		else if (*quote_type == 0)
			*quote_type = cur_quote;
	}
	if (has_unquoted)
		*quote_type = 0;
	return (result);
}
