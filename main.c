/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 19:02:13 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:45:51 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	handler(int sig)
{
	if (sig != SIGINT)
		return ;
	write(1, "minishell>\n", 11);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handler_here(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		exit(1);
	}
}

void	handler_fork(int sig)
{
	if (sig != SIGINT)
		return ;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int main(int ac, char **av, char **envp)
{
	char	*line;

	line = NULL;
	(void)ac;
	(void)av;
	while (1)
	{
		set_signal_quit_ig();
		set_signal_int_ex();
		signal(SIGINT,handler);
		line = readline("minishell>");
		if (line == NULL)
		{
			write(1, "exit\n", 5);
			//printf("현재 프로세스 아이디 {%d}", getpid());
			break ;
		}
		if (!ft_strlen(line))
			continue ;
		if (line)
		{
			add_history(line);
			ft_tokenizer(line, envp);
			free(line);
			line = NULL;
		}
	}
	return (0);
}