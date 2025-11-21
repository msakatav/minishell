/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 23:19:06 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:29:10 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_count_print(size_t count, int n, int negative)
{
	char	*num;
	int		tmp;

	tmp = n;
	while (tmp)
	{
		count++;
		tmp /= 10;
	}
	num = malloc(count + 1);
	if (!num)
		return (NULL);
	num[count] = '\0';
	while (n)
	{
		num[--count] = (n % 10) + '0';
		n /= 10;
	}
	if (negative)
		num[0] = '-';
	return (num);
}

char	*ft_itoa(int n)
{
	size_t	count;
	int		negative;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n == 0)
		return (ft_strdup("0"));
	count = 0;
	negative = 0;
	if (n < 0)
	{
		negative = 1;
		n = -n;
		count++;
	}
	return (ft_count_print(count, n, negative));
}
