/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:54:53 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:25:37 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	get_delimiter_count(const char *str, char c)
{
	int	count;
	int	ind;
	int	flag;

	count = 0;
	ind = 0;
	flag = 1;
	while (str[ind])
	{
		if (str[ind] != c && flag)
		{
			count++;
			flag = 0;
		}
		else
		{
			if (str[ind] == c)
				flag = 1;
		}
		ind++;
	}
	return (count);
}

char	*delimiter_string(char c)
{
	char	*char_str;

	char_str = (char *)malloc(sizeof(char) * 2);
	if (!char_str)
		return (NULL);
	char_str[0] = c;
	char_str[1] = '\0';
	return (char_str);
}

void	free_double_arr_str(char **str_arr)
{
	char	**temp;

	temp = str_arr;
	while (*str_arr)
	{
		free(*str_arr);
		*str_arr = NULL;
		str_arr++;
	}
	free(temp);
}

char	**extract_substrings(char **str_arr, char *old_str, char c)
{
	char	*new_str;
	char	**temp;

	temp = str_arr;
	new_str = ft_strchr(old_str, c);
	while (new_str)
	{
		*str_arr = ft_substr(old_str, 0, \
			ft_strlen(old_str) - ft_strlen(new_str));
		if (*str_arr == NULL)
			return (free_double_arr_str(str_arr), NULL);
		while (1)
		{
			if (*new_str != c)
				break ;
			new_str++;
		}
		old_str = new_str;
		new_str = ft_strchr(old_str, c);
		str_arr++;
	}
	*str_arr = ft_strdup(old_str);
	if (*str_arr == NULL)
		return (free_double_arr_str(str_arr), NULL);
	return (temp);
}

char	**ft_split(char const *s, char c)
{
	char	**str_arr;
	char	*old_str;
	char	*char_str;
	int		size;

	if (!s || !*s || c == '\0')
		return (NULL);
	char_str = delimiter_string(c);
	if (!char_str)
		return (NULL);
	old_str = ft_strtrim(s, char_str);
	free(char_str);
	if (!old_str)
		return (NULL);
	size = get_delimiter_count(old_str, c);
	str_arr = (char **)malloc(sizeof(char *) * (size + 1));
	if (!str_arr)
		return (free(old_str), NULL);
	extract_substrings(str_arr, old_str, c);
	free(old_str);
	if (!str_arr)
		return (NULL);
	return (str_arr[size] = NULL, str_arr);
}
