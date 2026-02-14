/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:08:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/14 12:18:02 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/*
solution-1
----------
limiter = ft_strjoin(limiter, "\n");
if ((ft_strlen(limiter) == ft_strlen(line)) &&
	(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0))

solution-2
----------
if ((line[ft_strlen(line)]=='\n') &&
	(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0))

however, solution-2 is more reliable and easy.   [OK]
solution-1, needs additional care like 1. creating str 2. free it after usage   [KO]
*/
static int	here_doc_input(char *limiter)
{
	int		fd1;
	int		fd2;
	char	*line;

	fd1 = open_file("/tmp/pipex_bonus_heredoc", 'w');
	fd2 = open_file("/tmp/pipex_bonus_heredoc", 'r');
	unlink("/tmp/pipex_bonus");
	write(STDOUT_FILENO, "> ", ft_strlen("> "));
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if ((line[ft_strlen(limiter)]=='\n') &&
		(ft_strncmp(line, limiter, ft_strlen(limiter)) == 0))
		{
			free(line);
			break ;
		}
		write(fd1, line, ft_strlen(line));
		write(STDOUT_FILENO, "> ", ft_strlen("> "));
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(fd1);
	return (fd2);
}

static void	edge_child_configuration(int f_fd, char *cmd, int pipefd[], \
	int edge)
{
	if (edge == 0)
	{
		close(pipefd[0]);
		if (dup2(f_fd, STDIN_FILENO) == -1)
			print_errors_and_exit("Pipex_Bonus: ", cmd, EX_FDFAILURE, 1);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			print_errors_and_exit("Pipex_Bonus: ", cmd, EX_FDFAILURE, 1);
	}
	else if (edge == 1)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			print_errors_and_exit("Pipex_Bonus: ", cmd, EX_FDFAILURE, 1);
		if (dup2(f_fd, STDOUT_FILENO) == -1)
			print_errors_and_exit("Pipex_Bonus: ", cmd, EX_FDFAILURE, 1);
	}
}

void	heredoc_edge_children_procs(char *limiter_or_f_path, int pipefd[], \
	char *cmd, int edge)
{
	int		f_fd;
	char	**cmd_split;
	char	*exe_path;

	if (edge == 0)
		f_fd = here_doc_input(limiter_or_f_path);
	else
		f_fd = open_file(limiter_or_f_path, 'a');
	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
		print_errors_and_exit("Pipex_Bonus: ", "", EX_CHDPROCFAILURE, 0);
	edge_child_configuration(f_fd, cmd_split[0], pipefd, edge);
	exe_path = executable_path(cmd_split);
	execve(exe_path, cmd_split, __environ);
	free(exe_path);
	print_errors_and_exit("Pipex_Bonus: ", cmd_split[0], 0, 1);
	free_strings_arr(cmd_split);
	exit(EX_CHDPROCFAILURE);
}

void	edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge)
{
	int		f_fd;
	char	**cmd_split;
	char	*exe_path;

	if (edge == 0)
		f_fd = open_file(f_path, 'r');
	else
		f_fd = open_file(f_path, 'w');
	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
		print_errors_and_exit("Pipex_Bonus: ", "", EX_CHDPROCFAILURE, 0);
	edge_child_configuration(f_fd, cmd_split[0], pipefd, edge);
	exe_path = executable_path(cmd_split);
	execve(exe_path, cmd_split, __environ);
	free(exe_path);
	print_errors_and_exit("Pipex_Bonus: ", cmd_split[0], 0, 1);
	free_strings_arr(cmd_split);
	exit(EX_CHDPROCFAILURE);
}

void	chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], \
	char *cmd)
{
	char	**cmd_split;
	char	*exe_path;

	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
		print_errors_and_exit("Pipex_Bonus: ", "", EX_CHDPROCFAILURE, 0);
	if (dup2(read_end_pipefd[0], STDIN_FILENO) == -1)
		print_errors_and_exit("Pipex_Bonus: ", cmd_split[0], EX_FDFAILURE, 1);
	if (dup2(write_end_pipefd[1], STDOUT_FILENO) == -1)
		print_errors_and_exit("Pipex_Bonus: ", cmd_split[0], EX_FDFAILURE, 1);
	close(read_end_pipefd[1]);
	close(write_end_pipefd[0]);
	exe_path = executable_path(cmd_split);
	execve(exe_path, cmd_split, __environ);
	free(exe_path);
	print_errors_and_exit("Pipex_Bonus: ", cmd_split[0], 0, 1);
	free_strings_arr(cmd_split);
	exit(EX_CHDPROCFAILURE);
}
