/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:42:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/14 11:57:51 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_env_path(char *env_key)
{
	char	**env;

	env = __environ;
	while (*env)
	{
		if (ft_strncmp(*env, env_key, ft_strlen(env_key)) == 0)
			return (ft_strdup(*env + 5));
		env++;
	}
	return (NULL);
}

/*
if _perror_errno=SUCCESS(0), we are not interested to print the error message,
only failure cases are infomed
if _perror_errno=-1, means invoked internal function has failed and
set the errno(no manual intervention needed)
*/
void	print_errors_and_exit(char *pre_text, char *msg_or_cmd, \
	int exitcode, int _perror_errno)
{
	char	*print_str;

	if (_perror_errno != -1 && _perror_errno != 0)
		errno = _perror_errno;
	if (_perror_errno)
	{
		print_str = ft_strjoin(pre_text, msg_or_cmd);
		perror(print_str);
		free(print_str);
	}
	else
	{
		write(2, pre_text, ft_strlen(pre_text));
		write(2, msg_or_cmd, ft_strlen(msg_or_cmd));
		write(2, "\n", 1);
	}
	if (exitcode != 0)
		exit(exitcode);
}

// @Child-Process-Method
int	open_file(const char *file, int write)
{
	int	fd;

	if (write)
		fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else
		fd = open(file, O_RDONLY);
	if (fd == -1)
		print_errors_and_exit("Pipex: ", (char *)file, EX_GENFAILURE, -1);
	return (fd);
}

// @Child-Process-Method
void	is_file_valid(const char *in_or_out, int outfile)
{
	if (outfile)
	{
		if (access(in_or_out, F_OK) == 0)
		{
			if (access(in_or_out, W_OK) != 0)
			{
				print_errors_and_exit("Pipex: ", (char *)in_or_out, \
				EX_OTFNW, EACCES);
			}
		}
	}
	else
	{
		if (access(in_or_out, F_OK) != 0)
			print_errors_and_exit("Pipex: ", (char *)in_or_out, \
			EX_ITFNF, ENOENT);
		else
		{
			if (access(in_or_out, R_OK) != 0)
			{
				print_errors_and_exit("Pipex: ", (char *)in_or_out, \
				EX_ITFNR, EACCES);
			}
		}
	}
}

void	free_strings_arr(char **str_arr)
{
	char	**temp;

	if (str_arr == NULL)
		return ;
	temp = str_arr;
	while (*temp)
	{
		free(*temp);
		*temp = NULL;
		temp++;
	}
	free(str_arr);
}
