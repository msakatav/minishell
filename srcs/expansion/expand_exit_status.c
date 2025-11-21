/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_exit_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:31:59 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_exit_status(char **result, int exit_status, int *i)
{
	char	*status_str;

	status_str = ft_itoa(exit_status);
	*result = ft_strjoin_free(*result, status_str, 3);
	*i += 2;
}

char	*expand_exit_status(char *str, int exit_status)
{
	char	*result;
	char	*tmp;
	int		i;
	int		start;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		start = i;
		while (str[i] && !(str[i] == '$' && str[i + 1] == '?'))
			i++;
		if (i > start)
		{
			tmp = ft_substr(str, start, i - start);
			result = ft_strjoin_free(result, tmp, 3);
		}
		if (str[i] == '$' && str[i + 1] == '?')
			append_exit_status(&result, exit_status, &i);
	}
	return (result);
}
