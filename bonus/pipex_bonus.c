/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:05 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:10:31 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static	void	pipex_bonus(int argc, char *argv[], t_pipex_params *p_prms, \
	void (*edge_child_processes)(char *, int[], char *, int));
static	void	exit_parent_process(t_list **children);

// ./pipex file1 cmd1 cmd2 file2
/*
It must take 4 arguments:
•file1 and file2 are file names.
•cmd1 and cmd2 are shell commands with their parameters

-------------------------------------------------------
2 Bonus Tasks:

1> Handle multiple pipes
2> Support « and » when the first parameter is "here_doc"

Error Handling:
---------------
1. infile: must have user read permission
2. outfile: must have user write permission

*/
// $> < file1 cmd1 | cmd2 > file2
int	main(int argc, char *argv[])
{
	char			*hd;
	t_pipex_params	pipex_parms;

	if (argc < 5)
		print_errors_and_exit("Pipex: Usage: ", "infile \"cmd1\"...\"cmdn*\" \
			outfile", EX_WARGS, 0);
	else
	{
		hd = "here_doc";
		if (ft_strncmp(argv[1], hd, ft_strlen(hd)) == 0)
		{
			if (argc < 6)
				print_errors_and_exit("Pipex: Usage: ", \
				"here_doc LIMITER \"cmd1 -flag*\" \"cmd2 -flag*\" ..\
				. \"cmd(n) -flag*\" outfile", EX_WARGS, 0);
			initialize_pipex_params(&pipex_parms, NULL, 3, argc - 2);
			pipex_bonus(argc, argv, &pipex_parms, heredoc_edge_children_procs);
		}
		else
		{
			initialize_pipex_params(&pipex_parms, NULL, 2, argc - 2);
			pipex_bonus(argc, argv, &pipex_parms, edge_children_procs);
		}
	}
	return (EX_SUCCESS);
}

static void	pipex_bonus(int argc, char *argv[], t_pipex_params *p_prms, \
	void (*edge_child_processes)(char *, int[], char *, int))
{
	while (p_prms->ind <= p_prms->cmd_end)
	{
		create_pipe(p_prms);
		p_prms->child = create_child_process(p_prms->pipefd[\
			p_prms->write_pipe], argv[p_prms->ind]);
		ft_lstadd_back(&(p_prms->children), \
		ft_lstnew((void *)(intptr_t)p_prms->child));
		if (p_prms->child == 0)
		{
			ft_lstclear(&(p_prms->children), child_del);
			if (p_prms->ind == p_prms->cmd_start)
				edge_child_processes(argv[p_prms->cmd_start - 1], \
					p_prms->pipefd[p_prms->write_pipe], argv[p_prms->ind], 0);
			else if (p_prms->ind > p_prms->cmd_start && \
					p_prms->ind < p_prms->cmd_end)
				chained_children_procs(p_prms->pipefd[!p_prms->write_pipe], \
					p_prms->pipefd[p_prms->write_pipe], argv[p_prms->ind]);
			else if (p_prms->ind == p_prms->cmd_end)
				edge_child_processes(argv[argc - 1], \
					p_prms->pipefd[!p_prms->write_pipe], argv[p_prms->ind], 1);
		}
		close_parent_pipefds(p_prms);
		p_prms->ind++;
	}
	exit_parent_process(&(p_prms->children));
}

static void	exit_parent_process(t_list **children)
{
	int		last_child_status;
	t_list	*next;

	next = *children;
	while (next->next != NULL)
	{
		waitpid((pid_t)(intptr_t)next->content, NULL, 0);
		next = next->next;
	}
	if (waitpid((pid_t)(intptr_t)next->content, &last_child_status, 0) == -1)
	{
		ft_lstclear(children, child_del);
		exit(EX_GENFAILURE);
	}
	ft_lstclear(children, child_del);
	if (WIFEXITED(last_child_status))
		exit(WEXITSTATUS(last_child_status));
	else if (WIFSIGNALED(last_child_status))
		exit(128 + WTERMSIG(last_child_status));
	else if (WIFSTOPPED(last_child_status))
		exit(WSTOPSIG(last_child_status));
	else
		exit(EX_GENFAILURE);
}
