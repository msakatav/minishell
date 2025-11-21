/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 00:00:00 by student           #+#    #+#             */
/*   Updated: 2025/11/22 05:31:59 by msakata          ###   ########TOKYO.jp  */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_input_redir(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		print_error(file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redir(char *file, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(file, flags, 0644);
	if (fd < 0)
	{
		print_error(file, strerror(errno));
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc_redir(char *file)
{
	int	heredoc_fd;

	heredoc_fd = handle_heredoc(file);
	if (heredoc_fd < 0)
		return (-1);
	if (dup2(heredoc_fd, STDIN_FILENO) < 0)
	{
		close(heredoc_fd);
		return (-1);
	}
	close(heredoc_fd);
	return (0);
}

static int	handle_single_redirection(t_redir *redir, t_data *data)
{
	if (redir->type == TOKEN_REDIR_IN)
	{
		if (handle_input_redir(redir->file) < 0)
			return (-1);
	}
	else if (redir->type == TOKEN_REDIR_OUT)
	{
		if (handle_output_redir(redir->file, 0) < 0)
			return (-1);
	}
	else if (redir->type == TOKEN_REDIR_APPEND)
	{
		if (handle_output_redir(redir->file, 1) < 0)
			return (-1);
	}
	else if (redir->type == TOKEN_REDIR_HEREDOC)
	{
		if (handle_heredoc_redir(redir->file) < 0)
			return (-1);
	}
	(void)data;
	return (0);
}

int	setup_redirections(t_redir *redirs, t_data *data)
{
	while (redirs)
	{
		if (handle_single_redirection(redirs, data) < 0)
			return (-1);
		redirs = redirs->next;
	}
	return (0);
}
