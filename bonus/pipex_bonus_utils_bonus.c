/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:00:56 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/09 13:03:14 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

// Do nothing - not malloc'd
void	child_del(void *content)
{
	(void)content;
}

void	initialize_pipex_params(t_pipex_params *p_prms, void *children, \
	int cmd_start, int cmd_end)
{
	p_prms->cmd_start = cmd_start;
	p_prms->children = children;
	p_prms->ind = p_prms->cmd_start;
	p_prms->cmd_end = cmd_end;
}

pid_t	create_child_process(int pipefd[], const char *cmd)
{
	pid_t	child;

	child = fork();
	if (child == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		print_errors_and_exit("Pipex_Bonus: ", (char *)cmd, EX_FORKFAILURE, 1);
	}
	return (child);
}

void	create_pipe(t_pipex_params *p_prms)
{
	p_prms->write_pipe = (p_prms->ind + 1) % 2;
	if (p_prms->ind < p_prms->cmd_end && \
		pipe(p_prms->pipefd[p_prms->write_pipe]) == -1)
		print_errors_and_exit("Pipex_Bonus: ", "", EX_PIPEFAILURE, 1);
}

void	close_parent_pipefds(t_pipex_params *p_prms)
{
	if (p_prms->ind == p_prms->cmd_start)
		close(p_prms->pipefd[p_prms->write_pipe][1]);
	else if (p_prms->ind > p_prms->cmd_start && p_prms->ind < p_prms->cmd_end)
	{
		close(p_prms->pipefd[!p_prms->write_pipe][0]);
		close(p_prms->pipefd[p_prms->write_pipe][1]);
	}
	else if (p_prms->ind == p_prms->cmd_end)
		close(p_prms->pipefd[!p_prms->write_pipe][0]);
}
