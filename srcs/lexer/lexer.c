/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 14:09:32 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	restore_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == 1)
			str[i] = ' ';
		else if (str[i] == 2)
			str[i] = '\t';
		else if (str[i] == 3)
			str[i] = '\n';
		else if (str[i] == 4)
			str[i] = '\v';
		else if (str[i] == 5)
			str[i] = '\f';
		else if (str[i] == 6)
			str[i] = '\r';
		i++;
	}
}

static void	add_split_tokens(t_token **tokens, char *value)
{
	int		i;
	int		start;
	char	*word;

	i = 0;
	while (value[i])
	{
		while (value[i] && ft_isspace(value[i]))
			i++;
		if (!value[i])
			break ;
		start = i;
		while (value[i] && !ft_isspace(value[i]))
			i++;
		word = ft_substr(value, start, i - start);
		restore_spaces(word);
		add_token(tokens, new_token(TOKEN_WORD, word));
	}
}

static void	process_word_token(char *input, int *i,
	t_token **tokens, t_data *data)
{
	char	*value;
	int		quote_type;

	value = build_combined_word(input, i, &quote_type, data);
	if (value)
	{
		if (!*value && quote_type != 0)
		{
			add_token(tokens, new_token(TOKEN_WORD, ft_strdup("")));
			free(value);
		}
		else if (*value)
		{
			add_split_tokens(tokens, value);
			free(value);
		}
		else
			free(value);
	}
}

static void	process_token(char *input, int *i, t_token **tokens, t_data *data)
{
	if (input[*i] == '|')
	{
		add_token(tokens, new_token(TOKEN_PIPE, ft_strdup("|")));
		(*i)++;
	}
	else if (input[*i] == '<' || input[*i] == '>')
		handle_redirect(input, i, tokens);
	else
		process_word_token(input, i, tokens, data);
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
