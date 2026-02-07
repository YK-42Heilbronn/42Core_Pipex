/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:49:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/07 14:28:13 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void		pipex(const char *file1, const char *cmd1, \
	const char *cmd2, const char *file2);
static pid_t	create_child_process(int pipefd[], const char *cmd);
static void		child_process_fd_setup(int not_used, int stdin, \
	int stdout, char **cmd_split);
static void		child_process(const char *cmd, const char *file, \
	int pipefd[], int child);

int	main(int argc, char *argv[])
{
	if (argc != 5)
	{
		print_errors_and_exit("Pipex: Usage: ", "infile \"cmd1 -flag1 -flag2\" \
			\"cmd2 -flag1 -flag2\" outfile", EX_WARGS, 0);
	}
	else
	{
		pipex(argv[1], argv[2], argv[3], argv[4]);
	}
	return (EX_SUCCESS);
}

static void	pipex(const char *file1, const char *cmd1, \
	const char *cmd2, const char *file2)
{
	int		pipefd[2];
	int		c2status;
	pid_t	child1;
	pid_t	child2;

	if (pipe(pipefd) == -1)
		print_errors_and_exit("Pipex: ", "", EX_PIPEFAILURE, -1);
	child1 = create_child_process(pipefd, cmd1);
	if (child1 == 0)
		child_process(cmd1, file1, pipefd, 0);
	child2 = create_child_process(pipefd, cmd2);
	if (child2 == 0)
		child_process(cmd2, file2, pipefd, 1);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child1, NULL, 0);
	waitpid(child2, &c2status, 0);
	if (WIFEXITED(c2status))
		exit(WEXITSTATUS(c2status));
	else if (WIFSIGNALED(c2status))
		exit(128 + WTERMSIG(c2status));
	else
		exit(WSTOPSIG(c2status));
}

static pid_t	create_child_process(int pipefd[], const char *cmd)
{
	pid_t	child;

	child = fork();
	if (child == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		print_errors_and_exit("Pipex: ", (char *)cmd, EX_FORKFAILURE, -1);
	}
	return (child);
}

static void	child_process_fd_setup(int not_used, int stdin, \
	int stdout, char **cmd_split)
{
	close(not_used);
	if (dup2(stdin, STDIN_FILENO) == -1)
		print_errors_and_exit("Pipex: ", cmd_split[0], EX_FDFAILURE, -1);
	if (dup2(stdout, STDOUT_FILENO) == -1)
		print_errors_and_exit("Pipex: ", cmd_split[0], EX_FDFAILURE, -1);
	close(stdin);
	close(stdout);
}

/*
child==0
    first_child_process
child==1
    last_child_process
*/
static void	child_process(const char *cmd, const char *file, \
	int pipefd[], int child)
{
	char	**cmd_split;
	char	*exe_path;
	int		f_fd;

	is_file_valid(file, child);
	f_fd = open_file(file, child);
	cmd_split = ft_split(cmd, ' ');
	if (cmd_split == NULL)
		print_errors_and_exit("Pipex: command not found: ", \
			cmd_split[0], EX_CHDPROCFAILURE, 0);
	exe_path = executable_path(cmd_split);
	if (child == 0)
		child_process_fd_setup(pipefd[0], f_fd, pipefd[1], cmd_split);
	if (child == 1)
		child_process_fd_setup(pipefd[1], pipefd[0], f_fd, cmd_split);
	execve(exe_path, cmd_split, __environ);
	free(exe_path);
	print_errors_and_exit("Pipex: ", cmd_split[0], 0, -1);
	free_strings_arr(cmd_split);
	exit(EX_APPFAILURE);
}
