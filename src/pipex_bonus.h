/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/03 20:26:51 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PIPEX_BONUS
# define PIPEX_BONUS

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "stdint.h"

# include "libft/libft.h"
# include "printf/ft_printf.h"
# include "getnextline/get_next_line.h"

// pipex_bonus.c
void pipex(int argc, char **argv);
// void first_child(pid_t f_child, const char *cmd, int f1_fd, int pipefd[]);
// void last_child(pid_t l_child, const char *cmd, int f2_fd, int pipefd[]);
// void multi_cmds(int argc, char *argv[]);
// void child_proc(pid_t child, const char *cmd, char *f_path, int arg, int pipefd[]);

// utils_bonus.c
int open_file_r(const char *file);
int open_file_w(const char *file);
int open_file_a(const char *file);
char *get_env_path();
char *get_exe_path(const char *env_path, const char *cmd);
void free_strings_arr(char **str_arr);

// multi_cmds.c
void multi_cmds(int argc, char *argv[]);
// void child_proc(pid_t child, const char *cmd, char *f_path, int arg, int pipefd[]);
void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd);
void edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge);
void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge);
void child_del(void *content);

// heredoc.c
void here_doc(int argc, char *argv[]);
// void child_del(void *content);
int here_doc_input(char *limiter);
// void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge);
void hd_edge_children_procs(char *limiter_or_f_path, int pipefd[], char *cmd, int edge);
// void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd);

# endif