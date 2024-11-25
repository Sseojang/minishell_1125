/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_qoute.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:25:10 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:47:38 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

int	ft_double_qoute(char *line, int i, char **envp)
{
	i++;
	while (line[i])
	{
		if (line[i] == 34)
			return (++i);
		else if(line[i] == '$')
			i += ft_export_check(line, i, envp);
		else
			write(1, &line[i++], 1);
	}
	write(1, "\nERROR", 6);
	exit(0);
}

int	ft_single_qoute(char *line, int i)
{
	i++;
	while (line[i])
	{
		if (line[i] == 39)
			return (++i);
		else
			write(1, &line[i], 1);
		i++;
	}
	write(1, "\nERROR", 6);
	exit(0);
}

void	ft_qoute_check(char *line, char **envp)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 39) //싱글 쿼트
		{
			i = ft_single_qoute(line, i);
		}
		else if (line[i] == 34) //더블 쿼트
		{
			i = ft_double_qoute(line, i, envp);
		}
		else if (line[i] == '$')
		{
			i += ft_export_check(line, i, envp);
		}
		else if (line[i] == '|')
		{
			write(1, "|", 1);
			i++;
		}
		else
			write(1, &line[i++], 1);
	}
}