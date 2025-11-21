/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 21:06:51 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:29:41 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_cpy(const unsigned char *s, unsigned char *d, size_t n)
{
	if (s < d && d < s + n)
	{
		d += n;
		s += n;
		while (n)
		{
			*(--d) = *(--s);
			n--;
		}
	}
	else
	{
		while (n)
		{
			*d++ = *s++;
			n--;
		}
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*s;
	unsigned char		*d;

	d = (unsigned char *)dest;
	s = (const unsigned char *)src;
	if (d == s || n == 0)
		return (dest);
	ft_cpy(s, d, n);
	return (dest);
}
