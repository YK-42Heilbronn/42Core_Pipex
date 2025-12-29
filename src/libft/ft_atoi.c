/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:10:56 by ykonka            #+#    #+#             */
/*   Updated: 2025/11/26 12:16:14 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void handler(int sig) {
//    write(0, "Caught signal", 13);
//    write(0, ft_itoa(sig), ft_strlen(ft_itoa(sig)));
// }

int	ft_atoi(const char *str)
{
	int		minus;
	long	result;

	// signal(SIGINT, handler);
	minus = 1;
	result = 0;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'
		|| *str == '\v' || *str == '\f')
		str++;
	if (*str == 45 || *str == 43)
	{
		minus = 44 - *str;
		str++;
	}
	// while (*str >= 48 && *str <= 57)
	while (*str)
		if (*str >= 48 && *str <= 57)
			result = (result * 10) + (*str++ - 48);
		else
			raise(SIGINT);
	return (result * minus);
}

