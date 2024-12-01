/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:08:50 by seojang           #+#    #+#             */
/*   Updated: 2024/12/01 16:00:14 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_paser_func(t_tokken_list **tokken, t_val *val)
{
	ft_val_set(val);
	//ft_find_pipe(*tokken, val, pipefd);
	ft_find_redir(tokken, val);
	if (val->here_sig == 1)
		return ;
	ft_find_cmd(*tokken, val);
}

void	ft_heredoc_change(t_tokken_list **tokken)
{
	t_tokken_list	*head;
	char	*doc_name;
	int		i;
	char	*doc_num;

	head = *tokken;
	i = 0;
	
	while (*tokken)
	{
		if (!ft_strncmp((*tokken)->content, "<<", 2))
		{
			printf("1. {%s}\n", (*tokken)->next->content);
			i += 1;
			doc_name = ft_strdup("a");
			doc_num = ft_strdup(ft_itoa(i));
			(*tokken)->next->content = ft_strdup(ft_strjoin(doc_name, doc_num));
			printf("2. {%s}\n", (*tokken)->next->content);
			(*tokken) = (*tokken)->next;
		}
		if (!(*tokken)->next)
			break;
		(*tokken) = (*tokken)->next;
	}
	*tokken = head;
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
	if (!here_flag)
	{
		set_signal_int_ig();
		pid_here = fork();
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
			exit(1);
		}
		else
		{
			wait(NULL);
		}
	}
	ft_heredoc_change(&tokken);
	set_signal_int_ex();
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
