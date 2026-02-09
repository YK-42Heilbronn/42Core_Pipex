/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/08 16:56:40 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "stdint.h"
# include "error.h"
# include "errno.h"

# include "../libs/getnextline/get_next_line.h"
# include "../libs/libft/libft.h"

typedef enum e_exit_codes{
	EX_SUCCESS = 0,
	EX_GENFAILURE = -1,
	EX_APPFAILURE = 1,
	EX_WARGS = 2,
	EX_ITFNF = 3,
	EX_ITFNX = 4,
	EX_ITFNR = 5,
	EX_OTFNF = 6,
	EX_OTFNX = 7,
	EX_OTFNW = 8,
	EX_FILEOPENFAILURE = 9,
	EX_FDREADFAILURE = 10,
	EX_FDFAILURE = 11,
	EX_CHDPROCFAILURE = 20,
	EX_FORKFAILURE = 30,
	EX_PIPEFAILURE = 40,
	EX_CMDNX = 126,
	EX_CMDNF = 127,
}	t_exit_codes;

typedef struct s_pipex_params{
	int		ind;
	int		cmd_start;
	int		cmd_end;
	int		pipefd[2][2];
	int		write_pipe;
	pid_t	child;
	t_list	*children;
}	t_pipex_params;

// pipex_bonus.c
// pipex_bonus_utils_bonus.c
void	child_del(void *content);
void	initialize_pipex_params(t_pipex_params *p_prms, void *children, \
		int cmd_start, int cmd_end);
pid_t	create_child_process(int pipefd[], const char *cmd);
void	create_pipe(t_pipex_params *p_prms);
void	close_parent_pipefds(t_pipex_params *p_prms);

// child_processes_bonus.c
void	chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], \
		char *cmd);
void	edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge);
void	heredoc_edge_children_procs(char *limiter_or_f_path, int pipefd[], \
		char *cmd, int edge);

// get_exe_path_bonus.c
char	*executable_path(char **cmd);

// utils_bonus.c
char	*get_env_path(char *env_key);
int		open_file(const char *file, char mode);
void	is_file_valid(const char *in_or_out, int infile);
void	print_errors_and_exit(char *pre_text, char *msg_or_cmd, \
		int exitcode, int _perror_errno);
void	free_strings_arr(char **str_arr);

#endif