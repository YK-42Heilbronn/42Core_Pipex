/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 10:34:38 by ykonka            #+#    #+#             */
/*   Updated: 2025/12/29 16:13:34 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PIPEX
# define PIPEX

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>

# include "libft/libft.h"
# include "printf/ft_printf.h"

// pipex.c
void pipex(const char *file1, const char *cmd1, const char *cmd2, const char *file2);

// utils.c
int open_file(const char *file);
char *get_env_path();
char *get_exe_path(const char *env_path, const char *cmd);
void free_strings_arr(char **str_arr);

# endif
