/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:08:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/07 16:12:44 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

// int here_doc_input(char *limiter){
//     int fd1;
//     int fd2;
//     char *line;
//     char *hd;
    
//     fd1 = open("/tmp/pipex_bonus", O_CREAT | O_WRONLY | O_TRUNC, 0700);
//     fd2 = open("/tmp/pipex_bonus", O_RDONLY, 0700);
//     // fd2 = dup(fd1);  // Which copies only fd1, not is struct file, which maintains f_pos
//     if (fd1 == -1 || fd2 == -1){
//         perror("here_doc/tmp file open failed");
//         exit(1);
//     }
//     unlink("/tmp/pipex_bonus");
    
//     hd = "pipex heredoc> ";
//     write(STDOUT_FILENO, hd, ft_strlen(hd));
//     printf("heredocing\n");
    
//     line = get_next_line(STDIN_FILENO);
//     printf("++++++++++\n");
//     printf("%s", line);
//     while((line = get_next_line(STDIN_FILENO))){
//         printf("%s\n", line);
//         if (ft_strncmp(line, limiter, ft_strlen(limiter))==0)
//             break;

//         write(fd1, line, ft_strlen(line));
//         free(line);
//         write(STDOUT_FILENO, hd, ft_strlen(hd));
//         // line = get_next_line(STDIN_FILENO);
//     }
//     printf("finished\n");
//     return (free(line), close(fd1), fd2);
// }

int here_doc_input(char *limiter){
    int fd1;
    int fd2;
    char *line;
    char *hd;
    printf("++++++++ entered to input func +++++++\n");
    fd1 = open("/tmp/pipex_bonus", O_CREAT | O_WRONLY | O_TRUNC, 0700);
    fd2 = open("/tmp/pipex_bonus", O_RDONLY, 0700);
    // fd2 = dup(fd1);  // Which copies only fd1, not is struct file, which maintains f_pos
    unlink("/tmp/pipex_bonus");

    // write(0, "entered to input func\n", ft_strlen("entered to input func\n"));
    printf("++++++++ tmp file opened to input func +++++++\n");
    fd1 = open("./bonus/sample.txt", O_RDWR, 0700);
    printf("%d\n", fd1);
    line = get_next_line(fd1);
    printf("======\n");
    while (line){
        printf("%s", line);
    }
    printf("======\n");
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

// static void heredoc_edge_child_configuration(char *limiter_or_f_path, int pipefd[], int edge){
//     int f_fd;
    
//     if (edge == 0){  // here_doc LIMITER
//         close(pipefd[0]);  // Read End
//         f_fd = here_doc_input(limiter_or_f_path);  // has implicit exit(1) upon failure
        
//         dup2(f_fd, STDIN_FILENO);
//         dup2(pipefd[1], STDOUT_FILENO);
        
//     }else if (edge == 1){  // file2
//         //close(pipefd[1]);  // Write End, for last pipe the write is already closed in parent itself
//         f_fd = open_file_a(limiter_or_f_path);
//         if (f_fd == -1){
//             perror("file1 open is failed");
//             exit(1);
//         }
        
//         dup2(pipefd[0], STDIN_FILENO);
//         dup2(f_fd, STDOUT_FILENO);
//     }
// }

void heredoc_edge_children_procs(char *limiter_or_f_path, int pipefd[], char *cmd, int edge){
    int f_fd;
    char **cmd_split;
    char *exe_path;
    printf("edge children procs\n");
    printf("limiter_or_f_path: %s || cmd: %s || edge: %d\n", limiter_or_f_path, cmd, edge);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);
    }
    if (edge == 0){  // here_doc LIMITER
        close(pipefd[0]);  // Read End
        printf("0000000 before heredoc input 00000000\n");
        f_fd = here_doc_input(limiter_or_f_path);
        printf("111111 after heredoc input 111111\n");
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
    // heredoc_edge_child_configuration(limiter_or_f_path, pipefd, edge);
    printf("---- end of here_doc_procs, edge: %d ----\n", edge);
    exe_path = get_env_path();
    if (exe_path == NULL){
        perror("here_doc exe_path fetch failed in edge child path");
        exit(1);
    }
    execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
}

static void edge_child_configuration(char *f_path, int pipefd[], int edge){
    int f_fd;
    
    if (edge==0)
        f_fd = open_file_r(f_path);
    else
        f_fd = open_file_w(f_path);
    if (f_fd == -1){
        perror("file1 open is failed");
        exit(1);
    }
    
    if (edge == 0){  // file1
        close(pipefd[0]);  // Read End
        
        dup2(f_fd, STDIN_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
    }else if (edge == 1){  // file2
        // close(pipefd[1]);  // Write End, for last pipe the write is already closed in parent itself
        
        dup2(pipefd[0], STDIN_FILENO);
        dup2(f_fd, STDOUT_FILENO);
    }
}

void edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge){
    // int f_fd;
    char **cmd_split;
    char *exe_path;
    
    printf("%s ------ edge children\n", cmd);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);
    }
    
    // if (edge==0)
    //     f_fd = open_file_r(f_path);
    // else
    //     f_fd = open_file_w(f_path);
    // if (f_fd == -1){
    //     perror("file1 open is failed");
    //     exit(1);
    // }
    
    // if (edge == 0){  // file1
    //     close(pipefd[0]);  // Read End
        
    //     dup2(f_fd, STDIN_FILENO);
    //     dup2(pipefd[1], STDOUT_FILENO);
    // }else if (edge == 1){  // file2
    //     // close(pipefd[1]);  // Write End, for last pipe the write is already closed in parent itself
        
    //     dup2(pipefd[0], STDIN_FILENO);
    //     dup2(f_fd, STDOUT_FILENO);
    // }
    edge_child_configuration(f_path, pipefd, edge);
    
    exe_path = get_env_path();
    if (exe_path == NULL){
        perror("multi_cmds exe_path fetch failed in edge child path");
        exit(1);
    }
    execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
}

void chained_children_procs(int read_end_pipefd[], int write_end_pipefd[], char *cmd){
    char **cmd_split;
    char *exe_path;

    printf("%s ------chained children\n", cmd);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);  // exit(EXIT_FAILURE);
    }
    // @TODO
    // close(read_end_pipefd[1]);  // Write End
    // close(write_end_pipefd[0]); // Read End
    
    dup2(read_end_pipefd[0], STDIN_FILENO);
    dup2(write_end_pipefd[1], STDOUT_FILENO);
    
    close(read_end_pipefd[1]);  // Write End
    close(write_end_pipefd[0]); // Read End
   
    exe_path = get_env_path();
    if (exe_path == NULL){
        perror("multi_cmds exe_path fetch failed in chained child path");
        exit(1);
    }
    execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
}

