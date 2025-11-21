/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 20:55:01 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:29:32 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*str;
	const unsigned char	*cpy;

	str = (unsigned char *)dest;
	cpy = (const unsigned char *)src;
	while (n)
	{
		*str = *cpy;
		str++;
		cpy++;
		n--;
	}
	return (dest);
}
