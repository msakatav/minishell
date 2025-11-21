/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:27:49 by msakata           #+#    #+#             */
/*   Updated: 2025/05/17 07:07:16 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	ft_nbr(size_t i, int neg, const char *nptr, long num)
{
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (num > (LONG_MAX - (nptr[i] - '0')) / 10)
		{
			if (neg == 1)
				return (LONG_MAX);
			else
				return (LONG_MIN);
		}
		num = num * 10 + (nptr[i] - '0');
		i++;
	}
	return (num * neg);
}

int	ft_atoi(const char *nptr)
{
	size_t	i;
	long	num;
	int		neg;

	num = 0;
	i = 0;
	neg = 1;
	while (nptr[i] == ' ' || nptr[i] == '\t'
		||nptr[i] == '\n' || nptr[i] == '\r'
		||nptr[i] == '\v' || nptr[i] == '\f')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
		{
			neg *= -1;
		}
		i++;
	}
	return ((int)(ft_nbr(i, neg, nptr, num)));
}
