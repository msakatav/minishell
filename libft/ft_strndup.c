/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 23:06:28 by msakata           #+#    #+#             */
/*   Updated: 2025/06/14 23:07:13 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *src, size_t n)
{
	char	*cpy;
	size_t	index;
	size_t	len;

	len = ft_strlen(src);
	if (len > n)
		len = n;
	cpy = malloc(len + 1);
	if (!cpy)
		return (NULL);
	index = 0;
	while (index < len)
	{
		cpy[index] = src[index];
		index++;
	}
	cpy[index] = '\0';
	return (cpy);
}
