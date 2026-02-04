/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:14:10 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/24 10:30:07 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s1_cpy;
	char	*tmp;

	if (!*s1)
		return NULL;
	s1_cpy = malloc(sizeof(char) * (ft_strlen(s1) + 1));
	tmp = s1_cpy;
	if (s1_cpy != NULL)
	{
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
	{
		return (NULL);
	}
}
