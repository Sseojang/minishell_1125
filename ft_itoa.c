/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojang <seojang@student.42gyeongsan.kr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 23:02:01 by seojang           #+#    #+#             */
/*   Updated: 2024/10/16 21:25:09 by seojang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ms_test.h"

static int	ft_ditcheck(int n)
{
	int	dit;

	dit = 0;
	if (n <= 0)
	{
		dit++;
		n *= (-1);
	}
	while (n > 0)
	{
		dit++;
		n /= 10;
	}
	return (dit);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		dit;
	long	result;

	if (n == (-2147483648))
		return (ft_strdup("-2147483648"));
	else if (n == 0)
		return (ft_strdup("0"));
	dit = ft_ditcheck(n);
	str = (char *)malloc(sizeof(char) * (dit + 1));
	if (!str)
		return (NULL);
	result = n;
	if (result < 0)
	{
		str[0] = '-';
		result *= (-1);
	}
	str[dit] = '\0';
	while (result > 0)
	{
		str[--dit] = result % 10 + 48;
		result /= 10;
	}
	return (str);
}
