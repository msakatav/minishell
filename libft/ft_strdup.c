/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:34:49 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:30:32 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *src)
{
	char	*cpy;
	size_t	index;
	size_t	len;

	len = ft_strlen(src);
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
