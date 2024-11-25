/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 17:25:52 by seojang           #+#    #+#             */
/*   Updated: 2024/11/25 21:48:44 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

	// 파이프로 스플릿 하는데 쿼트는 스킵 하면서 나누기
	// 파이프로 스플릿 후 파싱
	
	// pipex_bonus + 리다이렉션

char	*ft_redirection_check(char *line, int *i)
{
	char	*ptr;

	ptr = NULL;
	if (line[*i] == '<' && line[*i + 1] == '<' && line[*i + 2] != '<' && line[*i + 2] != '>')
	{
		ptr = ft_strdup("<<");
		(*i)++;
	}
	else if (line[*i] == '<' && line[*i + 1] != '<' && line[*i + 1] != '>')
		ptr = ft_strdup("<");
	else if (line[*i] == '>' && line[*i + 1] == '>' && line[*i + 2] != '<' && line[*i + 2] != '>')
	{
		ptr = ft_strdup(">>");
		(*i)++;
	}
	else if (line[*i] == '>' && line[*i + 1] != '<' && line[*i + 1] != '>')
		ptr = ft_strdup(">");
	else
	{
		printf("ERROR");
		exit(0);
	}
	return (ptr);
}

char	*ft_single_qoute_check(char *line, int *i)
{
	(*i)++;
	int		first_num;
	char	*ptr;

	ptr = NULL;
	first_num = *i;
	while (line[*i])
	{
		if (line[*i] == 39)
		{
			ptr = ft_substr(line, first_num, (*i) - first_num);
			break ;
		}
		(*i)++;
	}
	return (ptr);
}

char	*ft_export_push(char *temp, char **envp)
{
	int	i;
	int	j;
	char	*ret;
	int		first_num;

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
				first_num = j;
				while (envp[i][j] != '\0')
					j++;
				ret = ft_substr(envp[i], first_num, j);
				return (ret);
			}
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*ft_export_ptr(char *line, int *i, char **envp)
{
	char	*temp;
	char	*ret;
	int		first_num;

	first_num = ++(*i);
	if (line[*i] == '\0')
	{
		ret = ft_strdup("$");
		return (ret);
	}
	if (line[*i] == '?')
	{
		ret = ft_strdup("0");
		return (ret);
	}
	while (line[*i])
	{
		if (line[*i + 1] == ' ' || line[*i + 1] == '\t' || line[*i + 1] == '"' || line[*i + 1] == '\0' || line[*i + 1] == '$')
		{
			temp = ft_substr(line, first_num, *i + 1 - first_num);
			ret = ft_export_push(temp, envp);
			break ;
		}
		(*i)++;
	}
	return (ret);
}

char	*ft_double_qoute_check(char *line, int *i, char **envp)
{
	(*i)++;
	int		first_num;
	char	*ptr;
	char	*temp;

	ptr = NULL;
	first_num = *i;
	temp = NULL;
	if (line[*i] == 34)
		return (ft_strdup(""));
	while (line[*i])
	{
		if (line[*i] == 34)
		{
			if (!ptr && first_num != *i)
			{
				ptr = ft_substr(line, first_num, (*i) - first_num);
				break ;
			}
			temp = ft_substr(line, first_num, (*i) - first_num);
			ptr = ft_strjoin(ptr, temp);
			free(temp);
			break ;
		}
		else if (line[*i] == '$')
		{
			if (first_num < *i)
			{
				if (!ptr)
					ptr = ft_substr(line, first_num, (*i) - first_num);
				else
				{
					temp = ft_substr(line, first_num, (*i) - first_num);
					ptr = ft_strjoin(ptr, temp);
					temp = NULL;
				}
			}
			if (!ptr)
			{
				ptr = ft_strdup(ft_export_ptr(line, i, envp));
				first_num = (*i) + 1;
			}
			else
			{
				temp = ft_strdup(ft_export_ptr(line, i, envp));
				ptr = ft_strjoin(ptr, temp);
				free(temp);
				first_num = (*i) + 1;
			}
		}
		(*i)++;
	}
	return (ptr);
}

char	*ft_alpha_digit(char *line, int *i)
{
	char	*ptr;
	int		first_num;
	int		s_qoute_flag;
	int		d_qoute_flag;

	ptr = NULL;
	first_num = *i;
	d_qoute_flag = 0;
	s_qoute_flag = 0;
	while (line[*i])
	{
		if (ft_is_alpha(line[*i]) || ft_is_digit(line[*i]) || line[*i] == '-')
		{
			(*i)++;
		}
		else if (line[*i] == 34 && !d_qoute_flag)
		{
			d_qoute_flag = 1;
			(*i)++;
		}
		else if (d_qoute_flag && line[*i] == ' ')
		{
			(*i)++;
		}
		else if (d_qoute_flag && line[*i] == 34)
		{
			d_qoute_flag = 0;
			(*i)++;
		}
		else if (line[*i] == 39 && !s_qoute_flag)
		{
			s_qoute_flag = 1;
			(*i)++;
		}
		else if (s_qoute_flag && line[*i] == ' ')
		{
			(*i)++;
		}
		else if (s_qoute_flag && line[*i] == 39)
		{
			s_qoute_flag = 0;
			(*i)++;
		}
		else
			break ;
	}
	ptr = ft_substr(line, first_num, (*i) - first_num);
	--(*i);
	return (ptr);
}

char	*ft_option(char	*line, int *i)
{
	char	*ptr;
	int		first_num;

	++(*i);
	ptr = NULL;
	first_num = (*i);
	while (line[*i])
	{
		if (line[*i + 1] == ' ' || line[*i + 1] == '\t' || line[*i + 1] == 39 || line[*i + 1] == 34 || line[*i + 1] == '>' || line[*i + 1] == '<' || line[*i + 1] == '|' || line[*i + 1] == '-' || line[*i + 1] == '\0')
		{
			break ;
		}
		else
			(*i)++;
	}
	ptr = ft_strjoin(ft_strdup("-"),ft_substr(line, first_num, ((*i) + 1) - first_num));
	return (ptr);
}

char	*ft_space(int *i)
{
	char	*ptr;

	ptr = ft_strdup(" ");
	(*i)++;
	return (ptr);
}

void	ft_in_pipe(char *line, char **envp, t_tokken_list **tokken)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '<' || line[i] == '>')
			ft_lstadd_back(tokken, ft_lstnew(ft_redirection_check(line, &i)));
		else if (line[i] == 39) // single_qoute
			ft_lstadd_back(tokken, ft_lstnew(ft_single_qoute_check(line, &i)));
		else if (line[i] == 34) // double_qoute
			ft_lstadd_back(tokken, ft_lstnew(ft_double_qoute_check(line, &i, envp)));
		else if (line[i] == '$')
			ft_lstadd_back(tokken, ft_lstnew(ft_export_ptr(line, &i, envp)));
		else if (line[i] == '|')
			ft_lstadd_back(tokken, ft_lstnew(ft_strdup("|")));
		else if (ft_is_alpha(line[i]) || ft_is_digit(line[i]))
			ft_lstadd_back(tokken, ft_lstnew(ft_alpha_digit(line, &i)));
		else if (line[i] == '-')
			ft_lstadd_back(tokken, ft_lstnew(ft_option(line, &i)));
		else if (line[i] == ' ' || line[i] == '\t')
		{
			i++;
			continue ;
		}
		i++;
	}
}

void	ft_tokenizer(char *line, char **envp)
{
	t_tokken_list	*tokken;
	t_tokken_list	*lst;
	
	tokken = NULL;
	ft_qoute_check(line, envp);
	write(1, "\n", 1);
	ft_in_pipe(line, envp, &tokken);

	int	i = 0;
	lst = tokken;
	while (lst)
	{
		printf("ptr[%d] = %s\n", i, lst->content);
		lst = lst->next;
		i++;
	}
	ft_paser_manager(tokken, envp);
}