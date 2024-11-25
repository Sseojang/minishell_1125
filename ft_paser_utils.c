/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:15:18 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:50:21 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	error(char *s, int num)
{
	if (num == 1)
		write(2, s, ft_strlen(s));
	else if (num == 2)
		perror(s);
	else if (num == 3)
	{
		write(2, "command not found: ", ft_strlen("command not found: "));
		write(2, s, ft_strlen(s));
		write(2, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

void	ft_val_set(t_val *val)
{
	val->fd_in = -1;
	val->fd_out = -1;
	//val->prev_pipe = -1;
	//val->heredoc_fd = -1;
	//val->tokken_len = ft_lst_len(tokken);
}

void	ft_dup(t_val *val, char **envp, int *pipefd)
{
	if (!val->cmd)
		exit(EXIT_FAILURE);
	if (val->fd_in != -1)
	{
		close(pipefd[1]);
		if (dup2(val->fd_in, STDIN_FILENO) == -1)
			error("in dup2 failed", 2);
		close(pipefd[0]);
		close(val->fd_in);
	}
//	printf("before dup2 fd_out = {%d}\n", val->fd_out);
	//printf("현재 PID=%d outputfd값 : %d \n", getpid(), val->fd_out);
	if (val->fd_out != -1)
	{
		close(pipefd[0]);
		if (dup2(val->fd_out, STDOUT_FILENO) == -1)
			error("out dup2 failed", 2);
		close(pipefd[1]);
		close(val->fd_out);
	}
	execute_cmd(val->cmd, envp);
	exit(EXIT_FAILURE);
}
