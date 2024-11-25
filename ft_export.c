/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 16:41:39 by seojang           #+#    #+#             */
/*   Updated: 2024/09/24 20:00:35 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_print_export(char *temp, char **envp)
{
	int	i;
	int	j;
	i = 0;
	while (envp[i])
	{
		j = 0;
		if (envp[i][0] == temp[j])
		{
			while (envp[i][j] != '=')
			{
				if (envp[i][j] == temp[j])
					j++;
				else
					break ;
			}
			if (envp[i][j] == '=' && temp[j] == '\0') //변수 있음
			{
				j++;
				while (envp[i][j] != '\0')
					write(1, &envp[i][j++], 1);
				break ;
			}
		}
		i++;
	}
}

int	ft_export_check(char *line, int i, char **envp)
{
	char	*temp;
	int		first_num;

	first_num = ++i;
	temp = NULL;
	if (line[i] == '\0')
	{
		write(1, "$", 1);
		return (1);
	}
	if (line[i] == '?')
	{
		write(1, "0", 1);
		return (2);
	}
	while (line[i])
	{
		if (line[i + 1] == ' ' || line[i + 1] == '\t' || line[i + 1] == '"' || line[i + 1] == '\0' || line[i + 1] == '$')
		{
			temp = ft_substr(line, first_num, i + 1 - first_num);
			ft_print_export(temp, envp);
			return (i - first_num + 2);
		}
		i++;
	}
	exit(0);
}