/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/29 11:27:44 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_heredoc_filename(void)
{
	static int	i = 0;
	char		*num;
	char		*filename;

	num = ft_itoa(i++);
	filename = ft_strjoin("/tmp/.heredoc_", num);
	free(num);
	return (filename);
}

static void	read_heredoc_lines(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static void	read_heredoc_child(int fd, char *delimiter)
{
	signal(SIGINT, signal_handler_heredoc);
	read_heredoc_lines(fd, delimiter);
	close(fd);
	exit(0);
}

int	handle_heredoc(char *delimiter)
{
	char	*filename;
	int		fd;
	pid_t	pid;
	int		status;
	void	(*prev_handler)(int);

	filename = get_heredoc_filename();
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filename);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		read_heredoc_child(fd, delimiter);
	prev_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, prev_handler);
	close(fd);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		unlink(filename);
		free(filename);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	unlink(filename);
	free(filename);
	return (fd);
}
