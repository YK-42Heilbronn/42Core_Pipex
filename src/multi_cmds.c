/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 19:08:17 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/04 16:24:11 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

// void	ft_lstadd_back(t_list **lst, t_list *new)
// t_list	*ft_lstnew(void *content)

/* 
Pseudocode:
    args:: argc, argv[][]
    
    code::
        1> initialize: pipefd[argc-4][2], child, *children[linkedlist]
        2> argv[1] {argv[2], argv[3] ... argv[argc-2]} argv[argc-1]
              argv[1] pf[0][2]  pf[1][2]  pf[argc-5][2]
        
        3> ind = 2;
        4>  while(ind<=argc-2){
                a# child = fork()
                b#  if (child==0){
                        I.  creat new linkedlist node
                        II. add it to back of the linkedlist
                        III. 
                            |.  edge_children_proc(child, f_path, read_or_write_end_pipefd[], cmd, edge[0,1]) = child_proc(linkedlist->content, file1|file2, pf[0|argc-5], cmd, 0|1)
                            ||. chained_children_proc(child, read_end_pipefd[], write_end_pipefd[], cmd) = child_proc(linkedlist->content, pf[0][0], pf[1][1], cmd1)
                    }
                c# close parent pipefds || pipefd[ind][0], pipefd[ind][1]
                d# ind++
            }
        
        5> close opend fds
        6>  while(ind<list_size){
                waitpid(list[ind]->content, NULL, 0)
            }
            
    return:: void
*/
void multi_cmds(int argc, char *argv[]){
    // ./program file1 cmd1 cmd2 cmd3 ... cmdn-1 cmdn file2
    //      0      1    2    3    4   ... argc-3 argc-2 argc-1  --> total "argc" arguments
    int pipefd[argc-4][2];   // index=[0...argc-5]
    int ind;
    
    pid_t child;
    t_list *children;
    
    children = NULL;
    // child = NULL;
    
    ind = 2;  // cmd_1
    while (ind <= argc-2){  // till cmd_n
        if(ind<argc-2 && pipe(pipefd[ind-2])==-1){  // need (total_cmds - 1) pipes
            perror("pipe failed");
            exit(1);  // exit(EXIT_FAILURE)
        }
        
        child = fork();
        if (child == -1){
            close(pipefd[ind-2][0]);
            close(pipefd[ind-2][1]);
            perror("child1 failed");
            exit(1);   
        }
        ft_lstadd_back(&children, ft_lstnew((void*)(intptr_t)child));
        t_list *next;
        next = children;
        // printf("------------child- %d-------------\n", child);
        // while(next){
        //     printf("list child: %d\n", (int)(intptr_t)next->content);
        //     next = next->next;
        // }
        // printf("child: %d\n", child);
        if (child==0){
            // if (children == NULL)
            //     ft_lstadd_front(&children, ft_lstnew((void*)(intptr_t)child));
            // else
            // ft_lstadd_back(&children, ft_lstnew((void*)(intptr_t)child));
            // ft_lstclear(&children, free);
            if (ind == 2){                                               // cmd_1  // file1
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[1], 1, pipefd[ind-2]);
                // printf("child if 1, ind=%d\n", ind);
                edge_children_procs(argv[1], pipefd[ind-2], argv[ind], 0);
            }else if (ind > 2 && ind < argc-2){                         // cmd_2 ... cmd_n-1  // NULL
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], NULL, -1, pipefd[ind-2]);
                // printf("child if 2, ind=%d\n", ind);
                chained_children_procs(pipefd[ind-3], pipefd[ind-2], argv[ind]);
            }else if (ind == argc-2){                                   // cmd_n   // file2
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[argc-1], ind, pipefd[argc-5]);
                // printf("child if 3, ind=%d\n", ind);
                edge_children_procs(argv[argc-1], pipefd[ind-3], argv[ind], 1);
            }                                         
        }
        if (ind == 2){
            close_parent_pipefds(pipefd, ind-2, 0);
            // printf("--------parent pipefd if 1--------\n");
        }else if (ind > 2 && ind < argc-2){
            close_parent_pipefds(pipefd, ind-2, -1);
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
    // printf("------------child- %d-------------\n", child);
    // while(next){
    //     printf("list child: %d\n", (int)(intptr_t)next->content);
    //     next = next->next;
    // }
    exit(0);  // exit(EXIT_SUCCESS);
}

void child_del(void *content) {
    (void)content;  // Do nothing - not malloc'd
}

// 0: 1st, -1: not_edge, 1: nth
void close_parent_pipefds(int pipefd[][2], int n_pipe, int edge){
    
    if (edge == 0){
        close(pipefd[n_pipe][1]);  // Write End
        // ft_printf("pipefd if 1\n");
    } else if (edge == -1){
        close(pipefd[n_pipe-1][0]); // Read End
        close(pipefd[n_pipe][1]); // Write End
        // ft_printf("pipefd if 2\n");
    } else if (edge == 1){
        // ft_printf("pipefd if 3\n");
        if (n_pipe==0) // only in the case of 2 cmds
            close(pipefd[n_pipe][0]);  // Read End
        else
            close(pipefd[n_pipe-1][0]);  // Read End 
    }
}

void edge_children_procs(char *f_path, int pipefd[], char *cmd, int edge){
    int f_fd;
    // int f2_fd;
    char **cmd_split;
    char *exe_path;
    // printf("edge children procs\n");
    // printf("f_path: %s || cmd: %s || edge: %d\n", f_path, cmd, edge);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);
    }
    if (edge==0)
        f_fd = open_file_r(f_path);
    else
        f_fd = open_file_w(f_path);
    // printf("f_fd: %d, status: %d\n", f_fd, fcntl(f_fd, F_GETFD));
    // printf("pipefd: [%d, %d]\n", pipefd[0], pipefd[1]);
    // printf("[0]: %d = [1]: %d\n", fcntl(pipefd[0], F_GETFD), fcntl(pipefd[1], F_GETFD));
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
    // int fd;
    // printf("chained children procs\n");
    // printf("cmd: %s\n", cmd);
    close(read_end_pipefd[1]);
    close(write_end_pipefd[0]);

    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        perror("cmd split is failed");
        exit(1);  // exit(EXIT_FAILURE);
    }

    dup2(read_end_pipefd[0], STDIN_FILENO);
    dup2(write_end_pipefd[1], STDOUT_FILENO);

    close(read_end_pipefd[1]);
    close(write_end_pipefd[0]);
    
    exe_path = get_env_path();
    if (exe_path == NULL){
        perror("multi_cmds exe_path fetch failed in chained child path");
        exit(1);
    }
    execve(get_exe_path(exe_path, cmd_split[0]), cmd_split, __environ);
}

// void child_proc(pid_t child, const char *cmd, char *f_path, int arg, int pipefd[]){
//     char **cmd_split;
//     char *exe_path;
//     int fd;
    
//     if (child == 0){ // Child Process 2 -> cmd2
//         close(pipefd[1]);  // No need of Write End
        
//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd2_split failed");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
//         if (arg == 1 && f_path){
//             fd = open_file(f_path);  // file1close(pipefd[0]);
//             if (fd == -1){
//                 perror("file1 open failed");
//                 exit(1);
//             }
//             dup2(fd, STDIN_FILENO);  // f1_fd    
//             dup2(pipefd[1], STDOUT_FILENO); 
            
//             close(fd);  // f1_fd
//             close(pipefd[1]);
//         }else if (arg < 0 && f_path == NULL){
//             dup2(pipefd[0], STDIN_FILENO);   // Read End
//             dup2(pipefd[1], STDOUT_FILENO);  // Write End
            
//             close(pipefd[0]);
//             close(pipefd[1]);  
//         }else if (arg > 1 && f_path){
//             fd = open_file(f_path);  // file2
//             if (fd == -1){
//                 perror("file2 open failed");
//                 exit(1);
//             }
//             dup2(pipefd[0], STDIN_FILENO);    
//             dup2(fd, STDOUT_FILENO);  // f2_fd
            
//             close(fd);   // f2_fd
//             close(pipefd[0]);  
//         }
        
//         exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
//         if (exe_path == NULL){
//             perror("cmd2 exe_path NULL");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
//         execve(exe_path, cmd_split, __environ);
//     }
// }
