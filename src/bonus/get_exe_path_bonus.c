/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:01:18 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:07:51 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static char	*join_env_path_to_cmd(char *temp_paths_arr, const char *cmd, \
	char **paths_arr)
{
	char	*path_temp;
	char	*path;

	path_temp = ft_strjoin(temp_paths_arr, "/");
	if (path_temp == NULL)
		return (free_strings_arr(paths_arr), NULL);
	path = ft_strjoin(path_temp, cmd);
	free(path_temp);
	if (path == NULL)
		return (free_strings_arr(paths_arr), NULL);
	return (path);
}

static char	*get_exe_path(const char *env_path, const char *cmd)
{
	char	**paths_arr;
	char	**temp_paths_arr;
	char	*path;

	if (env_path == NULL || cmd == NULL || !*cmd)
		return (NULL);
	paths_arr = ft_split(env_path, ':');
	temp_paths_arr = paths_arr;
	free((char *)env_path);
	while (*temp_paths_arr)
	{
		path = join_env_path_to_cmd(*temp_paths_arr, cmd, paths_arr);
		if (path == NULL)
			return (free_strings_arr(paths_arr), NULL);
		if (access(path, F_OK) == 0)
			return (free_strings_arr(paths_arr), path);
		temp_paths_arr++;
		free(path);
		path = NULL;
	}
	return (free_strings_arr(paths_arr), NULL);
}

static void	cmd_access_check(char *cmd, char **cmd_split, int f_ok, int x_ok)
{
	if (f_ok)
	{
		if (access(cmd, F_OK) != 0)
		{
			print_errors_and_exit("Pipex_Bonus: ", cmd, 0, ENOENT);
			free_strings_arr(cmd_split);
			exit(EX_CMDNF);
		}
	}
	if (x_ok)
	{
		if (access(cmd, X_OK) != 0)
		{
			print_errors_and_exit("Pipex_Bonus: ", cmd, 0, EACCES);
			free_strings_arr(cmd_split);
			exit(EX_CMDNX);
		}
	}
}

static char	*handling_relative_and_home_cmds(char *cmd, \
	char **cmd_split, int relative)
{
	char	*cmd_path;

	if (relative)
		cmd_path = ft_strdup(cmd);
	else
		cmd_path = ft_strjoin(get_env_path("HOME="), cmd++);
	if (cmd_path == NULL)
	{
		print_errors_and_exit("Pipex: ", cmd, 0, ENOMEM);
		free_strings_arr(cmd_split);
		exit(EX_CMDNF);
	}
	if (!relative)
		cmd_access_check(cmd_path, cmd_split, 1, 1);
	return (cmd_path);
}

/*
1.if: relative path
2.if: user home path
3.if: environ path search
*/
// @Child-Process-Method
char	*executable_path(char **cmd_split)
{
	char	*cmd_path;
	char	*cmd;

	cmd = cmd_split[0];
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		cmd_access_check(cmd, cmd_split, 1, 1);
		return (handling_relative_and_home_cmds(cmd, cmd_split, 1));
	}
	if (cmd[0] == '~')
		return (handling_relative_and_home_cmds(cmd, cmd_split, 0));
	cmd_path = get_exe_path(get_env_path("PATH="), cmd);
	if (cmd_path == NULL)
	{
		print_errors_and_exit("Pipex: command not found: ", cmd, 0, 0);
		free_strings_arr(cmd_split);
		exit(EX_CMDNF);
	}
	else
		cmd_access_check(cmd_path, cmd_split, 0, 1);
	return (cmd_path);
}
