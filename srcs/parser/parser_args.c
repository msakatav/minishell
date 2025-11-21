/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 06:12:25 by msakata          ###   ########TOKYO.jp  */
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
		else if (tokens->type >= TOKEN_REDIR_IN
			&& tokens->type <= TOKEN_REDIR_HEREDOC)
		{
			tokens = tokens->next;
			if (!tokens)
				break ;
		}
		tokens = tokens->next;
	}
	return (count);
}

static void	process_arg_token(t_token **tokens,
	char **args, int *quotes, int *i)
{
	if ((*tokens)->type == TOKEN_WORD)
	{
		args[*i] = ft_strdup((*tokens)->value);
		quotes[*i] = (*tokens)->quote_type;
		(*i)++;
		*tokens = (*tokens)->next;
	}
	else if ((*tokens)->type >= TOKEN_REDIR_IN
		&& (*tokens)->type <= TOKEN_REDIR_HEREDOC)
	{
		*tokens = (*tokens)->next;
		if (*tokens)
			*tokens = (*tokens)->next;
	}
	else
		*tokens = (*tokens)->next;
}

char	**parse_args(t_token **tokens, int **quote_types)
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
		process_arg_token(tokens, args, quotes, &i);
	args[i] = NULL;
	quotes[i] = 0;
	*quote_types = quotes;
	return (args);
}
