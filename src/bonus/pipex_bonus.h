/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/06 16:13:39 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PIPEX_BONUS
# define PIPEX_BONUS

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "stdint.h"

# include "../libs/libft/libft.h"
# include "../libs/getnextline/get_next_line.h"

// pipex_bonus.c
void pipex_bonus(int argc, char **argv);
void multi_cmds(int argc, char *argv[]);
void here_doc(int argc, char *argv[]);

// child_processes_bonus.c
void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge);
void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd);
void edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge);
int here_doc_input(char *limiter);
void heredoc_edge_children_procs(char *limiter_or_f_path, int pipefd[], char *cmd, int edge);

// file_open_bonus.c
int open_file_r(const char *file);
int open_file_w(const char *file);
int open_file_a(const char *file);

// get_exe_path_bonus.c
char *get_env_path();
char *get_exe_path(const char *env_path, const char *cmd);
void free_strings_arr(char **str_arr);

// utils_bonus.c
void child_del(void *content);
void end_of_parent_proc(t_list **lst);

# endif