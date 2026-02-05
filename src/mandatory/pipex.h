/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 10:34:38 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/05 16:27:41 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include "error.h"
# include "errno.h"
# include "../libs/libft/libft.h"

typedef enum s_exit_codes{
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

// pipex.c
void	pipex(const char *file1, const char *cmd1, \
const char *cmd2, const char *file2);
// void	first_child(const char *cmd, const char *file1, int pipefd[]);
// void	last_child(const char *cmd, const char *file2, int pipefd[]);
void child_process(const char *cmd, const char *file, int pipefd[], int child);
pid_t	create_child_process(int pipefd[], const char *cmd);

// get_exe_path.c
char	*executable_path(char	**cmd_split);

// utils.c
void	print_errors(char *pre_text, char *msg_or_cmd, int _perror);
int		open_file(const char *file, int r_or_w);
int		is_file_exist(const char *path);
void	free_strings_arr(char **str_arr);
void	is_file_valid(const char *in_or_out, int infile);

#endif
