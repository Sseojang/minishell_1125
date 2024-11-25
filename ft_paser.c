/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:08:50 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:51:23 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_paser_func(t_tokken_list **tokken, t_val *val)
{
	ft_val_set(val);
	//ft_find_pipe(*tokken, val, pipefd);
	ft_find_redir(tokken, val);
	ft_find_cmd(*tokken, val);
}

void	ft_here_num(t_tokken_list *tokken, t_val *val)
{
	t_tokken_list *lst = tokken;
	
	val->here_num = 0;
	val->here_idx = 0;
	while (lst)
	{
		if (!ft_strncmp(lst->content, "<<", 2))
			val->here_num += 1;
		lst = lst->next;
	}
}

void	ft_paser_manager(t_tokken_list *tokken, char **envp)
{
	pid_t pid;
	pid_t pid_here;
	t_val val;
	int pipefd[2] = {-1, -1};
	int prev_pipe = -1;
	int status;
	int	here_flag;

	here_flag = 0;
	ft_here_num(tokken, &val);
	if (!here_flag && val.here_num > 0)
	{
		pid_here = fork();
		set_signal_int_ig();
		if (pid_here == 0)
		{
			set_signal_int_ex();
			signal(SIGINT, handler_here);
			val.doc_num = 0;
			if (!ft_strncmp(tokken->content, "<<", 2))
				ft_first_heredoc(&tokken, &val);
			else
				ft_heredoc(&tokken, &val);
			here_flag++;
		}
		else
		{
			wait(NULL);
		}
	}
	t_tokken_list *lst = tokken;
	int i = 0;
	while (lst)
	{
		printf("ptr[%d] = %s\n", i, lst->content);
		lst = lst->next;
		i++;
	}
	while (tokken)
	{
		if (ft_next_pipe(tokken))
		{
			if (pipe(pipefd) == -1)
				error("pipe failed", 1);
		}
		else
		{
			pipefd[0] = -1;
			pipefd[1] = -1;
		}
		ft_paser_func(&tokken, &val);
		//printf("플래그값 {%d}, 파일값{%d}", val.here_flag, val.fd_in);
		// if (val.here_flag == 1 && val.fd_in < 0)
		// 	break ;
		pid = fork();
		set_signal_quit_ex();
		signal(SIGQUIT,handler_quit);
		set_signal_int_ig();
		if (pid < 0)
			error("Fork error", 1);
		else if (pid == 0)
		{
			set_signal_int_ex();
			printf("fd in값 {%d} fd out값 {%d} token값 {%s}\n",  val.fd_in, val.fd_out, tokken->content);
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (pipefd[1] != -1)
			{
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
			if (pipefd[0] != -1)
				close(pipefd[0]);
			ft_dup(&val, envp, pipefd);
		}
		else
		{
			signal(SIGINT, handler_int);
			if (prev_pipe != -1)
				close(prev_pipe);
			if (pipefd[1] != -1)
				close(pipefd[1]);
			prev_pipe = pipefd[0];
			val.cmd = NULL; //cmd 초기화
		}
		while (tokken && tokken->content && ft_strncmp(tokken->content, "|", 1) != 0)
		{
			tokken = tokken->next;
		}
		if (tokken && tokken->content && ft_strncmp(tokken->content, "|", 1) == 0)
			tokken = tokken->next;
	}
	if (prev_pipe != -1)
		close(prev_pipe);
	while (wait(&status) > 0);
}
