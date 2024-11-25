/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser_util_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:08:54 by seojang           #+#    #+#             */
/*   Updated: 2024/10/17 17:32:41 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_find_pipe(t_tokken_list *tokken, t_val *val, int *pipefd)
{
	t_tokken_list	*lst;

	lst = tokken;
	while (lst)
	{
		if (lst->content && ft_strncmp(lst->content, "|", 1) == 0)
		{
			val->fd_out = pipefd[1];
			return ;
		}
		lst = lst->next;
	}
}

int	ft_next_pipe(t_tokken_list *tokken)
{
	t_tokken_list	*head;

	head = tokken;
	while (tokken && tokken->next != NULL)
	{
		if (tokken->content && ft_strncmp(tokken->content, "|", 1) == 0)
		{
			tokken = head;
			return (1);
		}
		tokken = tokken->next;
	}
	tokken = head;
	return (0);
}
