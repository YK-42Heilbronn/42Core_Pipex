/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/04 11:13:22 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef PIPEX_BONUS
# define PIPEX_BONUS

# include <fcntl.h>
# include "unistd.h"
# include <sys/types.h>
# include <sys/wait.h>
# include "stdint.h"
# include "error.h"
# include "errno.h"

# include "../libs/getnextline/get_next_line.h"
# include "../libs/libft/libft.h"

typedef enum {
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
} t_exit_codes;

// pipex_bonus.c
// void pipex_bonus(int argc, char **argv);
pid_t create_child_process(int pipefd[], const char *cmd);
void multi_cmds(int argc, char *argv[]);
void here_doc(int argc, char *argv[]);
void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge);

// child_processes_bonus.c
void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge);
void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd);
void edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge);
// int here_doc_input(char *limiter);
void heredoc_edge_children_procs(char *limiter_or_f_path, int pipefd[], char *cmd, int edge);

// file_open_bonus.c
int open_file_r(const char *file);
int open_file_w(const char *file);
int open_file_a(const char *file);
int is_file_exist(const char *file);
void is_file_valid(const char* in_or_out, int infile);

// get_exe_path_bonus.c
char *executable_path(char **cmd);

// utils_bonus.c
void child_del(void *content);
void end_of_parent_proc(t_list **lst);
void print_errors(char *pre_text, char *msg_or_cmd, int _perror);
void free_strings_arr(char **str_arr);

// ifs_split.c
char	**ifs_split(char const *s);

# endif