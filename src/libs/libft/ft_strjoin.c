/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:21:21 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:32:10 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*concat_str;
	char	*temp;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	concat_str = (char *)malloc(sizeof(char) * \
	(ft_strlen(s1) + ft_strlen(s2) + 1));
	if (concat_str == NULL)
		return (NULL);
	temp = concat_str;
	while (*s1)
	{
		*temp++ = *s1++;
	}
	while (*s2)
	{
		*temp++ = *s2++;
	}
	*temp = '\0';
	return (concat_str);
}
