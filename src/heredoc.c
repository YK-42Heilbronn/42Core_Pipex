/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:08:33 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/04 16:26:42 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

void here_doc(int argc, char *argv[]){
    // ./program here_doc LIMITER cmd1 cmd2 cmd3 ... cmdn-1 cmdn file2
    //      0       1        2      3    4    5  ... argc-3 argc-2 argc-1  --> total "argc" arguments
    // edge case with cmdn = 1
    int pipefd[argc-4][2];   // index=[0...argc-5]
    int ind;
    
    pid_t child;
    t_list *children;
    
    children = NULL;
    // child = NULL;
    
    ind = 3;  // cmd_1
    while (ind <= argc-2){  // till cmd_n
        if(ind<argc-2 && pipe(pipefd[ind-3])==-1){  // need (total_cmds - 1) pipes
            perror("pipe failed");
            exit(1);  // exit(EXIT_FAILURE)
        }
        
        child = fork();
        if (child == -1){
            close(pipefd[ind-3][0]);
            close(pipefd[ind-3][1]);
            perror("child1 failed");
            exit(1);   
        }
        ft_lstadd_back(&children, ft_lstnew((void*)(intptr_t)child));
        
        if (child==0){
            if (ind == 3){                                               // cmd_1  // file1
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[1], 1, pipefd[ind-2]);
                // printf("child if 1, ind=%d\n", ind);
                hd_edge_children_procs(argv[2], pipefd[ind-3], argv[ind], 0);
            }else if (ind > 3 && ind < argc-2){                         // cmd_2 ... cmd_n-1  // NULL
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], NULL, -1, pipefd[ind-2]);
                // printf("child if 2, ind=%d\n", ind);
                chained_children_procs(pipefd[ind-4], pipefd[ind-3], argv[ind]);
            }else if (ind == argc-2){                                   // cmd_n   // file2
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[argc-1], ind, pipefd[argc-5]);
                // printf("child if 3, ind=%d\n", ind);
                hd_edge_children_procs(argv[argc-1], pipefd[ind-4], argv[ind], 1);
            }                                         
        }
        // PARENT PROCESS 
        if (ind == 3){
            close_parent_pipefds(pipefd, ind-3, 0);
            // printf("--------parent pipefd if 1--------\n");
        }else if (ind > 3 && ind < argc-2){
            close_parent_pipefds(pipefd, ind-3, -1);
            // printf("--------parent pipefd if 2--------\n");
        }else if (ind == argc-2){
            close_parent_pipefds(pipefd, ind-3, 1);  // no pipe for cmd[argc-2]
            // printf("--------parent pipefd if 3--------\n");
        }
        ind++;
    }

    // close(pipefd[0]);
    // close(pipefd[1]);
    
    t_list *next;
    next = children;
    while(next){
        waitpid((pid_t)(intptr_t)next->content, NULL, 0);
        next = next->next;
    }
    ft_lstclear(&children, child_del);
    exit(0);  // exit(EXIT_SUCCESS);
}

int here_doc_input(char *limiter){
    int fd1;
    int fd2;
    char *line;
    char *hd;
    
    fd1 = open("/tmp/pipex_bonus", O_CREAT | O_WRONLY | O_TRUNC, 0700);
    fd2 = open("/tmp/pipex_bonus", O_RDONLY, 0700);
    // fd2 = dup(fd1);  // Which copies only fd1, not is struct file, which maintains f_pos
    unlink("/tmp/pipex_bonus");
    
    hd = "pipex heredoc> ";
    write(STDOUT_FILENO, hd, ft_strlen(hd));
    line = get_next_line(STDIN_FILENO);
    while(line){
        if (ft_strncmp(line, limiter, ft_strlen(limiter))==0){
            break;
        }
        write(fd1, line, ft_strlen(line));
        free(line);
        write(STDOUT_FILENO, hd, ft_strlen(hd));
        line = get_next_line(STDIN_FILENO);
    }
    free(line);
    close(fd1);
    
    return fd2;
}

void hd_edge_children_procs(char *limiter_or_f_path, int pipefd[], char *cmd, int edge){
    int f_fd;
    // int f2_fd;
    char **cmd_split;
    char *exe_path;
    // printf("edge children procs\n");
    // printf("limiter_or_f_path: %s || cmd: %s || edge: %d\n", limiter_or_f_path, cmd, edge);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);
    }
    
    if (edge == 0){  // here_doc LIMITER
        close(pipefd[0]);  // Read End
        f_fd = here_doc_input(limiter_or_f_path);
        
        dup2(f_fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
    }else if (edge == 1){  // file2
        //close(pipefd[1]);  // Write End, for last pipe the write is already closed in parent itself
        f_fd = open_file_a(limiter_or_f_path);
        if (f_fd == -1){
            perror("file1 open is failed");
            exit(1);
        }
        
        dup2(pipefd[0], STDIN_FILENO);
        dup2(f_fd, STDOUT_FILENO);
    }
    exe_path = get_env_path();
    if (exe_path == NULL){
        perror("here_doc exe_path fetch failed in edge child path");
        exit(1);
    }
    execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
}

// void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd){
//     char **cmd_split;
//     char *exe_path;
//     // int fd;
//     // printf("chained children procs\n");
//     // printf("cmd: %s\n", cmd);
//     close(read_end_pipefd[1]);
//     close(write_end_pipefd[0]);

//     cmd_split = ft_split(cmd, ' ');
//     if (cmd_split == NULL){
//         perror("cmd split is failed");
//         exit(1);  // exit(EXIT_FAILURE);
//     }

//     dup2(read_end_pipefd[0], STDIN_FILENO);
//     dup2(write_end_pipefd[1], STDOUT_FILENO);

//     close(read_end_pipefd[1]);
//     close(write_end_pipefd[0]);
    
//     exe_path = get_env_path();
//     if (exe_path == NULL){
//         perror("exe_path fetch failed in child path");
//         exit(1);
//     }
//     execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
// }
