/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 23:25:17 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:30:44 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	str_len;
	size_t	index;

	if (!s1 || !s2)
		return (NULL);
	str_len = ft_strlen(s1) + ft_strlen(s2);
	str = malloc(str_len + 1);
	if (!str)
		return (NULL);
	index = 0;
	while (*s1)
		str[index++] = *s1++;
	while (*s2)
		str[index++] = *s2++;
	str[index] = '\0';
	return (str);
}
