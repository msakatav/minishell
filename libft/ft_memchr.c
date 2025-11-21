/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:25:20 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:29:18 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*tmp = s;

	while (n)
	{
		if (*tmp == (unsigned char)c)
			return ((void *)tmp);
		tmp++;
		n--;
	}
	return (NULL);
}
