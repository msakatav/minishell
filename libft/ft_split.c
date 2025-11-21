/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msakata <msakata@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 23:22:28 by msakata           #+#    #+#             */
/*   Updated: 2025/05/14 22:30:20 by msakata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_copy(const char *s, size_t n)
{
	char	*str;

	str = malloc(n + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s, n);
	str[n] = '\0';
	return (str);
}

static int	ft_count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	if (c == '\0')
		return (1);
	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*ft_get_next_word(const char **s, char c)
{
	const char	*start;
	const char	*end;

	start = *s;
	while (*start == c)
		start++;
	end = start;
	while (*end && (*end != c))
		end++;
	*s = end;
	return (ft_copy(start, end - start));
}

static char	**ft_result(int count, char const *s, char c, char **result)
{
	int	index;

	index = 0;
	while (index < count)
	{
		result[index] = ft_get_next_word(&s, c);
		if (!result[index])
		{
			while (index > 0)
				free(result[--index]);
			free(result);
			return (NULL);
		}
		index++;
	}
	result[index] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		count;

	if (!s || *s == '\0')
	{
		result = malloc(sizeof(char *));
		if (!result)
			return (NULL);
		result[0] = NULL;
		return (result);
	}
	count = ft_count_words(s, c);
	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	return (ft_result(count, s, c, result));
}
