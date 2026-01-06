/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 10:34:38 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/06 16:11:01 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PIPEX
# define PIPEX

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>

# include "../libs/libft/libft.h"
// # include "lib/printf/ft_printf.h"

// pipex.c
void pipex(const char *file1, const char *cmd1, const char *cmd2, const char *file2);
void first_child(pid_t f_child, const char *cmd, int f1_fd, int pipefd[]);
void last_child(pid_t l_child, const char *cmd, int f2_fd, int pipefd[]);
void clean_fds(int f1_fd, int f2_fd, int pipefd[]);

// utils.c
int open_file(const char *file, int r_or_w);
char *get_env_path();
char *get_exe_path(const char *env_path, const char *cmd);
void free_strings_arr(char **str_arr);

# endif
