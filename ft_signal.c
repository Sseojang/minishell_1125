/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 20:59:56 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 16:49:41 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	set_signal_quit_ig(void)
// 현재 프로세스가 자식 프로세스를 생성하고 종료를 기다리는동안의 시그널 상태로 설정
{
	//signal(SIGINT, SIG_IGN); // ctrl+c 시그널 입력시 시그널을 무시함
	signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}

void	set_signal_quit_ex(void)
// 현재 프로세스가 시그널 동작을 기본 설정대로 동작하도록 원상복귀시켜줌
{
	//signal(SIGINT, SIG_DFL); // ctrl+c 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
	signal(SIGQUIT, SIG_DFL); // ctrl+\ 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
}

void	set_signal_int_ig(void)
// 현재 프로세스가 자식 프로세스를 생성하고 종료를 기다리는동안의 시그널 상태로 설정
{
	signal(SIGINT, SIG_IGN); // ctrl+c 시그널 입력시 시그널을 무시함
	//signal(SIGQUIT, SIG_IGN); // ctrl+\ 시그널 입력시 시그널을 무시함
}

void	set_signal_int_ex(void)
// 현재 프로세스가 시그널 동작을 기본 설정대로 동작하도록 원상복귀시켜줌
{
	signal(SIGINT, SIG_DFL); // ctrl+c 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
	//signal(SIGQUIT, SIG_DFL); // ctrl+\ 시그널 입력시 동작을 기본 시그널 설정으로 되돌림
}

void	handler_quit(int sig)
{
	if (sig == SIGQUIT)
	{
		write(1, "Quit (core dumped)\n", 19);
		return ;
	}
}

void	handler_int(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		return ;
	}
}