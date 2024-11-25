/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 21:37:09 by seojang           #+#    #+#             */
/*   Updated: 2024/09/05 17:24:47 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

void	flag1_case(t_word *word, char s, char c)
{
	if (word->single_quote_flag == 0 && s == '\'')
		word->single_quote_flag = 1;
	else if (word->double_quote_flag == 0 && s == '\"')
		word->double_quote_flag = 1;
	if (s != c)
	{
		word->count++;
		word->flag = 0;
	}
}

int	count_words(const char *s, char c)
{
	t_word	word;

	init_word(&word);
	while (*s)
	{
		if (word.flag == 1)
			flag1_case(&word, *s, c);
		else if (word.flag == 0 && word.single_quote_flag == 0 && \
				word.double_quote_flag == 0 && *s == c)
			word.flag = 1;
		else if (word.single_quote_flag == 1 && *s == '\'')
			word.single_quote_flag = 0;
		else if (word.double_quote_flag == 1 && *s == '\"')
			word.double_quote_flag = 0;
		s++;
	}
	if (word.single_quote_flag == 1 || word.double_quote_flag == 1)
		return (-1);
	return (word.count);
}

void	end_of_word(const char **s, char c, t_word *word)
{
	if (**s == '\'' || **s == '\"')
	{
		word->quote_flag = 1;
		(*s)++;
		word->start = *s;
		if (**s == '\'')
		{
			while (**s != '\'' && **s)
				(*s)++;
		}
		else
		{
			while (**s != '\"' && **s)
				(*s)++;
		}
		word->quote_flag = 0;
	}
	while (**s != c && **s)
		(*s)++;
	word->end = *s;
	if (is_quote(word->start - 1) && is_quote(word->end - 1))
		word->end --;
}

char	**split(const char *s, char c, int size, char **p)
{
	t_word	word;
	int		i;

	i = 0;
	word.quote_flag = 0;
	while (i < size)
	{
		while (*s == c && *s)
			s++;
		word.start = s;
		end_of_word(&s, c, &word);
		p[i] = (char *)malloc(sizeof(char) * ((word.end - word.start) + 1));
		if (!p[i])
		{
			ft_free(p, i);
			return (NULL);
		}
		ft_strlcpy(p[i], word.start, ((word.end - word.start) + 1));
		i++;
	}
	p[i] = 0;
	return (p);
}

char	**ft_split(const char *s, char c)
{
	char		**p;
	int			size;

	if (!s)
		return (NULL);
	size = count_words(s, c);
	if (size == -1)
		exit(EXIT_FAILURE);
	p = (char **)malloc(sizeof(char *) * (size + 1));
	if (!p)
		return (NULL);
	p = split(s, c, size, p);
	return (p);
}
