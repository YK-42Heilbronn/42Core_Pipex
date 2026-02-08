/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:00:56 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/08 16:38:13 by ykonka           ###   ########.fr       */
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

void close_parent_pipefds(t_pipex_params *p_prms, int write_pipe)
{
	// for 1st cmd, the read pipe is not used, no need to close, but close write end of the writepipe
	if (p_prms->ind == p_prms->cmd_start)
		close(p_prms->pipefd[write_pipe][1]); // Write end
	else if (p_prms->ind > p_prms->cmd_start && p_prms->ind < p_prms->cmd_end)
	{
		close(p_prms->pipefd[!write_pipe][0]); // last pipe read end
		close(p_prms->pipefd[write_pipe][1]); // curr pipe write end
	}
	else if (p_prms->ind == p_prms->cmd_end)
		close(p_prms->pipefd[!write_pipe][0]);  // Read end
}

// // 0: 1st, -1: not_edge, 1: nth
// static void	close_parent_pipefds(int pipefd[][2], int n_pipe, int edge)
// {
// 	if (edge == 0)
// 		close(pipefd[n_pipe][1]);  // Write end
// 	else if (edge == -1)
// 	{
// 		close(pipefd[n_pipe - 1][0]);  // last pipe read end
// 		close(pipefd[n_pipe][1]);  // curr pipe write end
// 	}
// 	else if (edge == 1)
// 	{
// 		if (n_pipe == 0)
// 			close(pipefd[n_pipe][0]);  // Read end
// 		else
// 			close(pipefd[n_pipe - 1][0]);  // Read end
// 	}
// }

// void unload_not_used_fds(t_pipex_params *p_prms, int pipefd[][2])
// {
// 	if (p_prms->ind == p_prms->cmd_start)
// 		close_parent_pipefds(pipefd, p_prms->ind - p_prms->cmd_start, 0);
// 	else if (p_prms->ind > p_prms->cmd_start && p_prms->ind < p_prms->cmd_end)
// 		close_parent_pipefds(pipefd, p_prms->ind - p_prms->cmd_start, -1);
// 	else if (p_prms->ind == p_prms->cmd_end)
// 		close_parent_pipefds(pipefd, p_prms->ind - p_prms->cmd_start, 1);
// }

