/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:05 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/02 14:53:57 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

// ./pipex file1 cmd1 cmd2 file2
/* 
It must take 4 arguments:
•file1 and file2 are file names.
•cmd1 and cmd2 are shell commands with their parameters

-------------------------------------------------------
2 Bonus Tasks:

1> Handle multiple pipes
2> Support « and » when the first parameter is "here_doc"

Error Handling:
---------------
1. infile: must have user read permission
2. outfile: must have user write permission

*/
// $> < file1 cmd1 | cmd2 > file2
int main(int argc, char *argv[]){
    char *hd;
    if (argc < 5){ 
        // ft_printf("wrong number of args");
        print_errors("Pipex: Usage: ", "infile \"cmd1 -flag*\" \"cmd2 -flag*\" ... \"cmdn -flag*\" outfile", 0);
        exit(EX_WARGS);
        // return(0);
    }else{
        // pipex_bonus(argc, argv);
        hd = "here_doc";
        if (ft_strncmp(argv[1], hd, ft_strlen(hd))==0){
            // $> ./pipex here_doc LIMITER cmd cmd1 outfile
            if (argc < 6){
                print_errors("Pipex: Usage: ", "here_doc LIMITER \"cmd1 -flag*\" \"cmd2 -flag*\" ... \"cmd(n) -flag*\" outfile", 0);
                exit(EX_WARGS);
            }
            here_doc(argc, argv);
        }else{
            multi_cmds(argc, argv);
        }
    }
    return(EX_SUCCESS);
}

// void pipex_bonus(int argc, char **argv){
//      char *hd;

//      hd = "here_doc";
//      if (ft_strncmp(argv[1], hd, ft_strlen(hd))==0){
//         if (argc < 6){
//             perror("not enough commands, must have atleast 2 commands");
//             exit(1);
//         }
//         here_doc(argc, argv);
//      }else{
//         // if (access(argv[1], R_OK)!=0){
//         //     perror("infile need user read permission");
//         //     exit(1);
//         // }
//         // if (access(argv[argc-1], W_OK)!=0){
//         //     perror("outfile need user write permission");
//         //     exit(1);
//         // }
//         multi_cmds(argc, argv);
//      }
// }

pid_t create_child_process(int pipefd[], const char *cmd){
    pid_t child;
    
    child = fork();
    if (child == -1){
        // clean_fds(pipefd);
        // perror("child1 failed");
        close(pipefd[0]), close(pipefd[1]);
        print_errors("Pipex_Bonus: fork failed: ", (char*)cmd, 1);
        exit(EX_FORKFAILURE);   
    }
    return child;
}

void multi_cmds(int argc, char *argv[]){
    // ./program file1 cmd1 cmd2 cmd3 ... cmdn-1 cmdn file2
    //      0      1    2    3    4   ... argc-3 argc-2 argc-1  --> total "argc" arguments
    int pipefd[argc-4][2];   // index=[0...argc-5]
    int ind;
    int last_child_status;
    
    pid_t child;
    t_list *children;
    
    children = NULL;
    
    ind = 2;  // cmd_1
    while (ind <= argc-2){  // till cmd_n
        if(ind<argc-2 && pipe(pipefd[ind-2])==-1){  // need (total_cmds - 1) pipes
            // perror("pipe failed");
            print_errors("Pipex_Bonus: ", "pipe failed: ", 1);
            exit(EX_PIPEFAILURE);  // exit(EXIT_FAILURE)
        }
        
        // child = fork();
        // if (child == -1){
        //     close(pipefd[ind-2][0]);
        //     close(pipefd[ind-2][1]);
        //     perror("child1 failed");
        //     exit(1);   
        // }
        // if (child>0)
        child=create_child_process(pipefd[ind-2], argv[ind]);
        ft_lstadd_back(&children, ft_lstnew((void*)(intptr_t)child));
        if (child==0){
            ft_lstclear(&children, child_del);
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
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    t_list *next;
    next = children;
    while(next->next != NULL){
        waitpid((pid_t)(intptr_t)next->content, NULL, 0);
        next = next->next;
    }
    // end_of_parent_proc(&children);
    // exit(0);  // exit(EXIT_SUCCESS);
    if (waitpid((pid_t)(intptr_t)next->content, &last_child_status, 0) == -1){
        ft_lstclear(&children, child_del);
        exit(EX_GENFAILURE);
    }
    ft_lstclear(&children, child_del);
    if (WIFEXITED(last_child_status))
        exit(WEXITSTATUS(last_child_status));
    else if (WIFSIGNALED(last_child_status))
        exit(128+WTERMSIG(last_child_status));
    else if (WIFSTOPPED(last_child_status))
        exit(WSTOPSIG(last_child_status));
    else
        exit(EX_GENFAILURE);
}

void here_doc(int argc, char *argv[]){
    // ./program here_doc LIMITER cmd1 cmd2 cmd3 ... cmdn-1 cmdn file2
    //      0       1        2      3    4    5  ... argc-3 argc-2 argc-1  --> total "argc" arguments
    // edge case with cmdn = 1
    int pipefd[argc-5][2];   // index=[0...argc-5]
    int ind;
    int last_child_status;
    
    pid_t child;
    t_list *children;
    
    children = NULL;
    
    ind = 3;  // cmd_1
    while (ind <= argc-2){  // till cmd_n
        if(ind<argc-2 && pipe(pipefd[ind-3])==-1){  // need (total_cmds - 1) pipes
            perror("pipe failed");
            exit(1);  // exit(EXIT_FAILURE)
        }
        
        // child = fork();
        // if (child == -1){
        //     close(pipefd[ind-3][0]);
        //     close(pipefd[ind-3][1]);
        //     perror("child1 failed");
        //     exit(1);   
        // }
        child = create_child_process(pipefd[ind-3], argv[ind]);
        ft_lstadd_back(&children, ft_lstnew((void*)(intptr_t)child));
        if (child==0){
            ft_lstclear(&children, child_del);
            if (ind == 3){                                               // cmd_1  // file1
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[1], 1, pipefd[ind-2]);
                // printf("child if 1, ind=%d\n", ind);
                heredoc_edge_children_procs(argv[2], pipefd[ind-3], argv[ind], 0);
            }else if (ind > 3 && ind < argc-2){                         // cmd_2 ... cmd_n-1  // NULL
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], NULL, -1, pipefd[ind-2]);
                // printf("child if 2, ind=%d\n", ind);
                chained_children_procs(pipefd[ind-4], pipefd[ind-3], argv[ind]);
            }else if (ind == argc-2){                                   // cmd_n   // file2
                // child_proc((pid_t)(intptr_t)ft_lstlast(children)->content, argv[ind], argv[argc-1], ind, pipefd[argc-5]);
                // printf("child if 3, ind=%d\n", ind);
                heredoc_edge_children_procs(argv[argc-1], pipefd[ind-4], argv[ind], 1);
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
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    t_list *next;
    next = children;
    while(next->next != NULL){
        waitpid((pid_t)(intptr_t)next->content, NULL, 0);
        next = next->next;
    }
    // end_of_parent_proc(&children);
    // exit(0);  // exit(EXIT_SUCCESS);
    waitpid((pid_t)(intptr_t)next->content, &last_child_status, 0);
    ft_lstclear(&children, child_del);
    if (WIFEXITED(last_child_status))
        exit(WEXITSTATUS(last_child_status));
    else if (WIFSIGNALED(last_child_status))
        exit(128+WTERMSIG(last_child_status));
    else if (WIFSTOPPED(last_child_status))
        exit(WSTOPSIG(last_child_status));
    else
        exit(EX_GENFAILURE);
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

