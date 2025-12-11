/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/12/11 14:51:27 by msakata          ###   ########TOKYO.jp  */
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

static void	read_heredoc_child(int fd, char *delimiter, t_data *data,
		char *filename)
{
	signal(SIGINT, signal_handler_heredoc);
	read_heredoc_lines(fd, delimiter);
	close(fd);
	free(filename);
	cleanup_data(data);
	if (g_signal_received == SIGINT)
		exit(130);
	exit(0);
}

static int	wait_heredoc_child(pid_t pid)
{
	int		status;
	void	(*prev_handler)(int);

	prev_handler = signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, prev_handler);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		return (1);
	return (0);
}

int	create_heredoc(char *delimiter, char **filename_out, t_data *data)
{
	char	*filename;
	int		fd;
	pid_t	pid;

	filename = get_heredoc_filename();
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(filename);
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		read_heredoc_child(fd, delimiter, data, filename);
	close(fd);
	if (wait_heredoc_child(pid))
	{
		unlink(filename);
		free(filename);
		return (-1);
	}
	*filename_out = filename;
	return (0);
}
