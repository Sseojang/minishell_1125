/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paser_util_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:25:40 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:31:54 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	ft_push_doc(t_tokken_list **tokken, t_val *val)
{
	//char	*prev_cmd;
	char	*eof;
	char	*ret;
	char	*temp;
	char	*file;
	char	*doc_name;
	char	*doc_num;
	int		tmpfd;
	int		i;

	i = 0;
	val->doc_num += 1;
	doc_name = ft_strdup("a");
	doc_num = ft_strdup(ft_itoa(val->doc_num));
	doc_name = ft_strjoin(doc_name, doc_num);
	file = ft_strdup(doc_name);
	eof = ft_strdup((*tokken)->next->next->content);
	temp = NULL;
	ret = NULL;
	val->here_idx += 1;
	while (1)
	{
		temp = readline(">");
		if (temp == NULL)
		{
			printf("경고 파일의 끝으로 구분함 (`%s'아(가) 필요함)\n", eof);
			//히어독 개수 세서 마지막 히어독일 경우 exit로 나오게 할 것
			//printf("{%d} {%d}\n", val->here_num, val->here_idx);
			if (val->here_num == val->here_idx)
			{
				val->here_num = 0;
				val->here_idx = 0;
				exit(1);
			}
			else
				return ;
		}
		if (temp && !ft_strncmp(temp, eof, ft_strlen(eof)))
		{
			if (!i)
				ret = ft_strdup("");
			break ;
		}
		if (ft_strncmp(temp, "\n", 1))
			temp = ft_strjoin(temp, ft_strdup("\n"));
		if (!ret)
			ret = ft_strdup(temp);
		else
			ret = ft_strjoin(ret, ft_strdup(temp));
		i++;
	}
	tmpfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(tmpfd, ret, ft_strlen(ret));
	close(tmpfd);
	(*tokken)->next->next->content = ft_strdup(file);
	free(file);
}

void	ft_heredoc(t_tokken_list **tokken, t_val *val)
{
	t_tokken_list	*head;

	head = (*tokken);
	while (*tokken && (*tokken)->next)
	{
		if (!ft_strncmp((*tokken)->next->content, "<<", 2))
			ft_push_doc(tokken, val);
		(*tokken) = (*tokken)->next;
	}
	
	(*tokken) = head;
}

void	ft_push_first_doc(t_tokken_list **tokken, t_val *val)
{
	//char	*prev_cmd;
	char	*eof;
	char	*ret;
	char	*temp;
	char	*file;
	char	*doc_name;
	char	*doc_num;
	int		tmpfd;
	int		i;

	i = 0;
	val->doc_num += 1;
	doc_name = ft_strdup("a");
	doc_num = ft_strdup(ft_itoa(val->doc_num));
	doc_name = ft_strjoin(doc_name, doc_num);
	file = ft_strdup(doc_name);
	eof = ft_strdup((*tokken)->next->content);
	temp = NULL;
	ret = NULL;
	val->here_idx += 1;
	while (1)
	{
		temp = readline(">");
		if (temp == NULL)
		{
			printf("경고 파일의 끝으로 구분함 (`%s'아(가) 필요함)\n", eof);
			//히어독 개수 세서 마지막 히어독일 경우 exit로 나오게 할 것
			//printf("{%d} {%d}\n", val->here_num, val->here_idx);
			if (val->here_num == val->here_idx)
			{
				val->here_num = 0;
				val->here_idx = 0;
				exit(1);
			}
			else
				return ;
		}
		if (temp && !ft_strncmp(temp, eof, ft_strlen(eof)))
		{
			if (!i)
				ret = ft_strdup("");
			break ;
		}
		if (ft_strncmp(temp, "\n", 1))
			temp = ft_strjoin(temp, ft_strdup("\n"));
		if (!ret)
			ret = ft_strdup(temp);
		else
			ret = ft_strjoin(ret, ft_strdup(temp));
		i++;
	}
	tmpfd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(tmpfd, ret, ft_strlen(ret));
	close(tmpfd);
	(*tokken)->next->content = ft_strdup(file);
	free(file);
}

void	ft_first_heredoc(t_tokken_list **tokken, t_val *val)
{
	t_tokken_list	*head;

	head = (*tokken);
	while (*tokken && (*tokken)->next)
	{
		if (!ft_strncmp((*tokken)->content, "<<", 2))
			ft_push_first_doc(tokken, val);
		(*tokken) = (*tokken)->next;
	}
	(*tokken) = head;
}