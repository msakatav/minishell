/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 07:43:59 by root              #+#    #+#             */
/*   Updated: 2025/06/22 14:12:21 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *old, int old_size, int new_size)
{
	void	*new_arr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(old);
		return (NULL);
	}
	new_arr = malloc(new_size);
	if (!new_arr)
		return (NULL);
	if (old)
	{
		if (old_size < new_size)
			copy_size = old_size;
		else
			copy_size = new_size;
		ft_memcpy(new_arr, old, copy_size);
		free(old);
	}
	return (new_arr);
}
