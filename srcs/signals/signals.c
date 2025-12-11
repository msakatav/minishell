/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/11 14:39:23 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

void	signal_handler(int signum)
{
	g_signal_received = signum;
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		g_signal_received = signum;
		write(1, "\n", 1);
		close(STDIN_FILENO);
	}
}

void	setup_signals(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
