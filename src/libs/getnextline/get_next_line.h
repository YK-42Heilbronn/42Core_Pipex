/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 17:58:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/16 17:12:34 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

char	*get_next_line(int fd);
size_t	md_strlen(const char *str);
char	*md_strchr(const char *s, int c);
char	*md_strjoin(char const *s1, char const *s2);
char	*md_substr(char const *s, unsigned int start, size_t len);
char	*md_strdup(const char *s1);

#endif
