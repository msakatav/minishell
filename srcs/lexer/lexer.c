/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 07:11:35 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
