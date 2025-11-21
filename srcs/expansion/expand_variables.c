/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/20 18:32:37 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	if (str[*i] >= '0' && str[*i] <= '9')
		return (NULL);
	if (!ft_isalnum(str[*i]) && str[*i] != '_')
		return (NULL);
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

static char	*get_var_value(char *var_name, t_data *data)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
	{
		value = ft_itoa(data->exit_status);
		return (value);
	}
	value = get_env_value(data->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

static char	*expand_single_var(char *str, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = get_var_name(str, i);
	if (!var_name || !*var_name)
	{
		if (var_name)
			free(var_name);
		return (ft_strdup("$"));
	}
	var_value = get_var_value(var_name, data);
	free(var_name);
	return (var_value);
}

char	*expand_variables(char *str, t_data *data)
{
	char	*result;
	char	*tmp;
	char	*var_value;
	int		i;
	int		start;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		start = i;
		while (str[i] && str[i] != '$')
			i++;
		if (i > start)
		{
			tmp = ft_substr(str, start, i - start);
			result = ft_strjoin_free(result, tmp, 3);
		}
		if (str[i] == '$')
		{
			var_value = expand_single_var(str, &i, data);
			result = ft_strjoin_free(result, var_value, 3);
		}
	}
	return (result);
}
