/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:44:32 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/16 18:26:23 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*initialize_or_join(char **rest_of_buf_ptr, char **buf_ptr,
				int initialize);
static void	clean_memory(char **rest_of_buf_ptr, char **buf_ptr);
static char	*return_line(char *next_line, char **rest_of_buf_ptr,
				char **buf_ptr);
static char	*end_of_file(char **rest_of_buf_ptr, char **buf_ptr,
				int bytes_read);

char	*get_next_line(int fd)
{
	static char	*rest_of_buffer;
	char		*buffer;
	char		*next_line;
	int			bytes_read;

	next_line = NULL;
	buffer = initialize_or_join(&rest_of_buffer, &buffer, 1);
	if (!buffer)
		return (NULL);
	while (1)
	{
		next_line = md_strchr(rest_of_buffer, '\n');
		if (!next_line)
		{
			bytes_read = read(fd, buffer, BUFFER_SIZE);
			if (bytes_read >= 0)
				buffer[bytes_read] = '\0';
			if (bytes_read <= 0)
				return (end_of_file(&rest_of_buffer, &buffer, bytes_read));
			if (!initialize_or_join(&rest_of_buffer, &buffer, 0))
				return (NULL);
		}
		else
			return (return_line(next_line, &rest_of_buffer, &buffer));
	}
}

static char	*initialize_or_join(char **rest_of_buf_ptr, char **buf_ptr,
		int initialize)
{
	char	*buffer_or_joined;

	if (initialize)
	{
		buffer_or_joined = malloc(sizeof(char) * (BUFFER_SIZE + 1));
		if (!buffer_or_joined || BUFFER_SIZE <= 0)
		{
			if (*rest_of_buf_ptr)
				clean_memory(rest_of_buf_ptr, NULL);
			return (NULL);
		}
		return (buffer_or_joined);
	}
	else
	{
		buffer_or_joined = md_strjoin(*rest_of_buf_ptr, *buf_ptr);
		free(*rest_of_buf_ptr);
		*rest_of_buf_ptr = NULL;
		if (buffer_or_joined && (*buffer_or_joined != '\0'))
			return (*rest_of_buf_ptr = buffer_or_joined, "");
		return (clean_memory(NULL, buf_ptr), NULL);
	}
}

static void	clean_memory(char **rest_of_buf_ptr, char **buf_ptr)
{
	if (rest_of_buf_ptr && *rest_of_buf_ptr)
	{
		free(*rest_of_buf_ptr);
		*rest_of_buf_ptr = NULL;
	}
	if (buf_ptr && *buf_ptr)
	{
		free(*buf_ptr);
		*buf_ptr = NULL;
	}
}

static char	*return_line(char *next_line, char **rest_of_buf_ptr,
		char **buf_ptr)
{
	char		*sub_str;
	char		*temp;
	long int	len;
	len = next_line - *rest_of_buf_ptr + 1;
	sub_str = md_substr(*rest_of_buf_ptr, 0, len);
	if (!sub_str || sub_str[0] == '\0')
	{
		clean_memory(rest_of_buf_ptr, buf_ptr);
		free(sub_str);
		return (NULL);
	}
	temp = md_strdup(*rest_of_buf_ptr + len);
	if (!temp)
		return (clean_memory(rest_of_buf_ptr, buf_ptr), free(sub_str), NULL);
	free(*rest_of_buf_ptr);
	*rest_of_buf_ptr = temp;
	if (buf_ptr && *buf_ptr)
	{
		free(*buf_ptr);
		*buf_ptr = NULL;
	}
	return (sub_str);
}

static char	*end_of_file(char **rest_of_buf_ptr, char **buf_ptr, int bytes_read)
{
	char	*last_line;

	if (bytes_read == -1)
	{
		clean_memory(rest_of_buf_ptr, buf_ptr);
		return (NULL);
	}
	last_line = NULL;
	if (rest_of_buf_ptr && *rest_of_buf_ptr)
	{
		last_line = md_strdup(*rest_of_buf_ptr);
		clean_memory(rest_of_buf_ptr, buf_ptr);
		if (!last_line)
			return (last_line);
		if (last_line[0] == '\0')
		{
			free(last_line);
			last_line = NULL;
		}
		return (last_line);
	}
	clean_memory(rest_of_buf_ptr, buf_ptr);
	return (NULL);
}
