/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/09 20:25:49 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_data(t_data *data, char **envp)
{
	data->env = init_env(envp);
	data->cmds = NULL;
	data->exit_status = 0;
	data->stdin_backup = -1;
	data->stdout_backup = -1;
}

static void	process_line(char *line, t_data *data)
{
	t_token	*tokens;
	int		i;

	if (!line || !*line)
		return ;
	i = 0;
	while (line[i] && ft_isspace(line[i]))
		i++;
	if (!line[i])
		return ;
	add_history(line);
	tokens = lexer(line, data);
	if (!tokens)
		return ;
	data->cmds = parser(tokens, data);
	free_tokens(tokens);
	if (data->cmds)
	{
		executor(data->cmds, data);
		free_cmds(data->cmds);
		data->cmds = NULL;
	}
}

static void	shell_loop(t_data *data)
{
	char	*line;

	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			ft_putendl_fd("exit", 2);
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			data->exit_status = 130;
			g_signal_received = 0;
		}
		process_line(line, data);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argc;
	(void)argv;
	init_data(&data, envp);
	setup_signals();
	shell_loop(&data);
	cleanup_data(&data);
	rl_clear_history();
	return (data.exit_status);
}
