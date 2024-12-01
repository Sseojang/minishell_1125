/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser_util_redir.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:11:47 by seojang           #+#    #+#             */
/*   Updated: 2024/12/01 15:59:53 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_find_redir(t_tokken_list **tokken, t_val *val)
{
	t_tokken_list	*lst;

	lst = *tokken;
	while (lst)
	{
		if (lst->content)
		{
			if (!ft_strncmp(lst->content, ">", 1) && ft_strlen(lst->content) == 1)
			{
				ft_redir_out(lst, val, tokken);
			}
			else if (!ft_strncmp(lst->content, ">>", 2) && ft_strlen(lst->content) == 2)
			{
				ft_redir_add(lst, val, tokken);
			}
			else if (!ft_strncmp(lst->content, "<", 1) && ft_strlen(lst->content) == 1)
			{
				ft_redir_open(lst, val, tokken);
			}
			else if (!ft_strncmp(lst->content, "<<", 2) && ft_strlen(lst->content) == 2)
			{
				ft_redir_here(lst, val, tokken);
				if (val->here_sig == 1)
					return ;
			}
			else if (!ft_strncmp(lst->content, "|", 1))
				break ;
		}
		lst = lst->next;
	}
}

void	ft_redir_open(t_tokken_list *lst, t_val *val, t_tokken_list **tokken)
{
	char	*file;
	t_tokken_list	*head;
	int	i;

	i = 0;
	head = (*tokken);
	if (!lst->next || !lst->next->content)
		error("redir next cmd error", 1);
	file = ft_strdup(lst->next->content);
	if (!file || !ft_strncmp(file, "|", 1))
		error("redir error", 1);
	val->fd_in = open(file, O_RDONLY);
	if (val->fd_in < 0)
	{
		write(1, "input error\n", 12);
		return ;
	}
	while ((*tokken) && ft_strncmp((*tokken)->content, "<", 1))
		(*tokken) = (*tokken)->next;
	while ((*tokken) && ft_strncmp((*tokken)->content, "|", 1))
	{
		(*tokken)->content = ft_strdup("");
		(*tokken) = (*tokken)->next;
		i++;
		if (i == 2)
			break ;
	}
	(*tokken) = head;
}

void	ft_redir_out(t_tokken_list *lst, t_val *val, t_tokken_list **tokken)
{
	char	*file;
	t_tokken_list	*head;
	int	i;

	i = 0;
	head = (*tokken);
	if (!lst->next || !lst->next->content)
		error("redir next cmd error", 1);
	file = ft_strdup(lst->next->content);
	if ( !file || !ft_strncmp(file, "|", 1))
		error("redir error", 1);
	val->fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	//printf("outfile fd_out = {%d}\n", val->fd_out);
	if (val->fd_out < 0)
		error("output error", 1);
	while ((*tokken) && ft_strncmp((*tokken)->content, ">", 1))
		(*tokken) = (*tokken)->next;
	while ((*tokken) && ft_strncmp((*tokken)->content, "|", 1))
	{
		(*tokken)->content = ft_strdup("");
		(*tokken) = (*tokken)->next;
		i++;
		if (i == 2)
			break ;
	}
	(*tokken) = head;
}

void	ft_redir_add(t_tokken_list *lst, t_val *val, t_tokken_list **tokken)
{
	char	*file;
	t_tokken_list	*head;
	int	i;
	
	i = 0;
	head = (*tokken);
	if (!lst->next || !lst->next->content)
		error("redir next cmd error", 1);
	file = ft_strdup(lst->next->content);
	if ( !file || !ft_strncmp(file, "|", 1))
		error("redir error", 1);
	val->fd_out = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (val->fd_out < 0)
		error("output error", 1);
	while ((*tokken) && ft_strncmp((*tokken)->content, ">>", 2))
		(*tokken) = (*tokken)->next;
	while ((*tokken) && ft_strncmp((*tokken)->content, "|", 1))
	{
		(*tokken)->content = ft_strdup("");
		(*tokken) = (*tokken)->next;
		i++;
		if (i == 2)
			break ;
	}
	(*tokken) = head;
}

void	ft_redir_here(t_tokken_list *lst, t_val *val, t_tokken_list **tokken)
{
	char	*file;
	t_tokken_list	*head;
	int	i;

	i = 0;
	head = (*tokken);
	if (!lst->next || !lst->next->content)
	{
		write(1, "redir next cmd error\n", 21);
		return ;
	}
	file = ft_strdup(lst->next->content);
	if (!file || !ft_strncmp(file, "|", 1))
		error("redir error", 1);
	val->fd_in = open(file, O_RDONLY);
	//printf("before heredoc file fd_in = {%d}\n", val->fd_in);
	if (val->fd_in < 0)
	{
		val->here_sig = 1;
		return ;
	}
	while ((*tokken) && ft_strncmp((*tokken)->content, "<<", 2))
		(*tokken) = (*tokken)->next;
	while ((*tokken) && ft_strncmp((*tokken)->content, "|", 1))
	{
		(*tokken)->content = ft_strdup("");
		(*tokken) = (*tokken)->next;
		i++;
		if (i == 2)
			break ;
	}
	unlink(file);
	(*tokken) = head;
}
