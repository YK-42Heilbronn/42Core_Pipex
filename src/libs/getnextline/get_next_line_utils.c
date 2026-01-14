/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 18:00:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/07 16:34:55 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	count;

	count = 0;
	if (str)
	{
		while (*str++)
			count++;
	}
	return (count);
}

char	*ft_strchr(const char *s, int c)
{
	printf("  strchr\n");
	if (s != NULL)
	{
		while (*s)
		{
			printf("    strchr while\n");
			if (*s == (char)c)
				return ((char *)s);
			s++;
		}
		if (*s == (char)c)
		{
			return ((char *)s);
		}
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concat_str;
	char	*temp;

	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	concat_str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)
				+ 1));
	if (concat_str == NULL)
		return (NULL);
	temp = concat_str;
	if (s1)
	{
		while (*s1)
			*temp++ = *s1++;
	}
	if (s2)
	{
		while (*s2)
			*temp++ = *s2++;
	}
	*temp = '\0';
	return (concat_str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_str;
	int		ind;

	if (!s)
		return (NULL);
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	ind = 0;
	if (start < ft_strlen(s))
	{
		sub_str = malloc((len + 1) * sizeof(char));
		if (!sub_str)
			return (NULL);
		while (s[start] && len-- > 0)
			sub_str[ind++] = s[start++];
	}
	else
	{
		sub_str = (char *)malloc(1 * sizeof(char));
		if (!sub_str)
			return (NULL);
	}
	sub_str[ind] = '\0';
	return (sub_str);
}

char	*ft_strdup(const char *s1)
{
	char	*s1_cpy;
	char	*tmp;

	if (!s1)
		return (NULL);
	s1_cpy = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	if (s1_cpy != NULL)
	{
		tmp = s1_cpy;
		while (*s1)
		{
			*s1_cpy = *s1;
			s1++;
			s1_cpy++;
		}
		*s1_cpy = '\0';
		return (tmp);
	}
	else
		return (NULL);
}
