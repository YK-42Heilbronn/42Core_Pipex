/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:49:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/14 14:29:37 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

// ./pipex file1 cmd1 cmd2 file2
/* 
It must take 4 arguments:
•file1 and file2 are file names.
•cmd1 and cmd2 are shell commands with their parameters
-------------------------------------------------------
write(0, "stdin\n", 6);
write(1, "stdout\n", 7);
write(2, "stderror\n", 9); 
*/
// $> < file1 cmd1 | cmd2 > file2
int main(int argc, char *argv[]){
       if (argc != 5){
            // ft_printf("wrong number of args");
            print_errors("Pipex: Usage: ", "infile \"cmd1 -flag1 -flag2\" \"cmd2 -flag1 -flag2\" outfile", 0);
            // print_error("Pipex: Usage: ", "infile cmd1 cmd2 outfile", 0);
            exit(EX_WARGS);
       }else{
            // parse_argvs_files(argv[1], argv[4]);
            pipex(argv[1], argv[2], argv[3], argv[4]);
       }
    return(EX_SUCCESS);
}

/* 
Pseudocode:
    while(multi_cmd_execution>1){
        1. parse the args
        2. build pipeline
        3. check memory sharing workflow
        4. create processes
        5. create child processes
        6. execute the commands
        7. wait processes status change
        8. exit process
    }

Mechanism:
    open()

    readend() + access()

    writeend() + access()
    
    pipe()

    fork()

    execve()

    wait(), waitpid()

    exit()

System Error Numbers: [https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/errno.h.html]
    fork() : if failed @details.txt
        [EAGAIN] - Resource unavailable, try again (may be the same value as [EWOULDBLOCK])
        [ENOMEM] - Not enough space
        [EPERM] - Operation not permitted
    pipe() : if failed @details.txt
        [EMFILE] - File descriptor value too large
        [ENFILE] - Too many files open in system
        [ENOMEM] - Not enough space
        [EPIPE] - Broken pipe [IF read/write to pipefds FAILED]
    dup2() : if failed @man dup2
        [EBADF] - oldfd isn't an open file descriptor
        [EBADF] - newfd is out of the allowed range for file descriptors
        [EBUSY] - (Linux only) This may be returned by dup2() or dup3() during a race condition with open(2) and dup()
        [EINTR] - The dup2() or dup3() call was interrupted by a signal
        [EINVAL] - (dup3()) flags contain an invalid value
        [EINVAL] - (dup3()) oldfd was equal to newfd
        [EMFILE] - The per-process limit on the number of open file descriptors has been reached

*/
void pipex(const char *file1, const char *cmd1, const char *cmd2, const char *file2){
    // int f1_fd;
    // int f2_fd;
    int pipefd[2];
    int c1status;
    int c2status;
    
    pid_t child1;
    pid_t child2;
    
    // char **cmd_split;
    // cmd_exe_path1 = exe_path(cmd1);
    // cmd_exe_path2 = exe_path(cmd2);
    
    // @REFACTOR - its child process
    // f1_fd = open_file(file1, 1); // read only
    // if (f1_fd == -1){
    //     // print_errors("Pipex: ", file1, 1); // @ALREADY INDICATED IN open_file() function
    //     exit(EX_FILEOPENFAILURE);
    // }
    // @REFACTOR - its child process
    // f2_fd = open_file(file2, 0);  // write only
    // if (f2_fd == -1){
    //     close(f1_fd);
    //     // perror("file2 open failed");  // @ALREADY INDICATED IN open_file() function
    //     exit(EX_FILEOPENFAILURE);
    // }

    // cmd_split = NULL;
    // ft_printf("cmd2: %s\n", cmd2);
    
    if(pipe(pipefd)==-1){
        // close(f1_fd);
        // close(f2_fd);
        // perror("pipe failed");
        print_errors("Pipex: ", "pipe create failed", 1);
        exit(EX_PIPEFAILURE);  // exit(EXIT_FAILURE)
    }
    
    // child1 = fork();
    // if (child1 == -1){
    //     clean_fds(f1_fd, f2_fd, pipefd);
    //     // perror("child1 failed");
    //     print_errors("Pipex: ", "fork failed", 1);
    //     exit(EX_FORKFAILURE);   
    // }
    if ((child1=create_child_process(pipefd, cmd1))==0)
        first_child(cmd1, file1, pipefd);
    
    // if (child1 == 0){ // Child Process 1 -> cmd1
    //     close(pipefd[0]); // No need of Read End
        
    //     cmd_split = ft_split(cmd1, ' ');
    //     if (cmd_split == NULL){
    //         perror("cmd1_split failed");
    //         exit(1);  // exit(EXIT_FAILURE);
    //     }
        
    //     dup2(f1_fd, STDIN_FILENO);       // setting fd=0 to f1_fd open_file_discription instead terminal/keyboard
    //     dup2(pipefd[1], STDOUT_FILENO); // setting fd=1 to pipe write_end struct_file instead terminal/monitor
        
    //     close(f1_fd); // delete original fd
    //     close(pipefd[1]);  // Write End is successfully copied, delete original fd
        
    //     exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
    //     if (exe_path == NULL){
    //         perror("cmd1 exe_path NULL");
    //         exit(1);  // exit(EXIT_FAILURE);
    //     }
    //     execve(exe_path, cmd_split, __environ);
    // }
    
    // child2 = fork();
    // if (child2 == -1){
    //     clean_fds(f1_fd, f2_fd, pipefd);
    //     print_errors("Pipex: ", "fork failed", 1);
    //     exit(EX_FORKFAILURE);   
    // }
    if ((child2=create_child_process(pipefd, cmd2) == 0))
        last_child(cmd2, file2, pipefd);
    
    // if (child2 == 0){ // Child Process 2 -> cmd2
    //     close(pipefd[1]);  // No need of Write End
        
    //     cmd_split = ft_split(cmd2, ' ');
    //     if (cmd_split == NULL){
    //         perror("cmd2_split failed");
    //         exit(1);  // exit(EXIT_FAILURE);
    //     }
        
    //     dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
    //     // dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
    //     dup2(f2_fd, STDOUT_FILENO);
        
    //     close(f2_fd);  // delete original fd
    //     close(pipefd[0]);  // Read End is successfully copied, delete it
    //     exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
    //     if (exe_path == NULL){
    //         perror("cmd2 exe_path NULL");
    //         exit(1);  // exit(EXIT_FAILURE);
    //     }
    //     execve(exe_path, cmd_split, __environ);
    // }
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(child1, &c1status, 0);
    waitpid(child2, &c2status, 0);
    char *cleaned_cmd;
    cleaned_cmd = NULL;
    // if (WIFEXITED(c1status) && WEXITSTATUS(c1status) != 0){
    //     cleaned_cmd = ft_strtrim(cmd1, " \t\v\n\r\f");
    //     // print_errors("Pipex: ", cleaned_cmd, 0);
    //     free(cleaned_cmd);
    //     // exit(WEXITSTATUS(c1status));
    // } else if (WIFSIGNALED(c1status)){
    //     cleaned_cmd = ft_strtrim(cmd1, " \t\v\n\r\f");
    //     print_errors("Pipex: ", cleaned_cmd, 0);
    //     free(cleaned_cmd);
    // }
    
    if (WIFEXITED(c2status)){
        // cleaned_cmd = ft_strtrim(cmd2, " \t\v\n\r\f");
        // // print_errors("Pipex: ", cleaned_cmd, 1);
        // free(cleaned_cmd);
    //     exit(WEXITSTATUS(c2status));
    // }else{
        // print_errors("Pipex: ", "child2", 1);
        exit(WEXITSTATUS(c2status));
    } else if (WIFSIGNALED(c2status)){
        // cleaned_cmd = ft_strtrim(cmd2, " \t\v\n\r\f");
        // print_errors("Pipex c2 killed: ", cleaned_cmd, 1);
        // free(cleaned_cmd);
        exit(WTERMSIG(c2status));
    }else
        exit(WSTOPSIG(c2status));
    // close(pipefd[0]);
    // close(pipefd[1]);
    // close(f1_fd);
    // close(f2_fd);
   
    // exit(0);  // exit(EXIT_SUCCESS);
}

// void clean_fds(int f1_fd, int f2_fd, int pipefd[]){
//     close(f1_fd);
//     close(f2_fd);
//     close(pipefd[0]);
//     close(pipefd[1]);
// }

pid_t create_child_process(int pipefd[], const char *cmd){
    pid_t child;
    
    child = fork();
    if (child == -1){
        // clean_fds(pipefd);
        // perror("child1 failed");
        close(pipefd[0]), close(pipefd[1]);
        print_errors("Pipex: fork failed: ", (char*)cmd, 1);
        exit(EX_FORKFAILURE);   
    }
    return child;
}

// static char **cmd_split_arr(const char *cmd){
//     char **cmd_split;
//     cmd_split = ft_split(cmd, ' ');
//     if (cmd_split == NULL){
//         perror("cmd1_split failed");
//         // exit(EX_CHDPROCFAILURE);  // exit(EXIT_FAILURE);
//         return NULL;
//     }
//     return(cmd_split);
// }

/* 
f1_fd:
    if fails: exit()

exe_path:
    if fails: exit()

close(pipefd[1]):
    pipe() creat failure is very rare and pipefds get -1 case also seldom
*/
void first_child(const char *cmd, const char *file1, int pipefd[]){
    // pid_t first_child;
    char **cmd_split;
    char *exe_path;
    int f1_fd;
    
    is_file_valid(file1, 1);
    f1_fd = open_file(file1, 1); // read only
    // if (f1_fd == -1){
    //     // print_errors("Pipex: ", file1, 1); // @ALREADY INDICATED IN open_file() function
    //     // exit(EX_FILEOPENFAILURE);
    // }
    
//    f_child = fork();
    // if (f_child == 0){ // Child Process 1 -> cmd1
    close(pipefd[0]); // No need of Read End

    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        // perror("cmd1_split failed");
        print_errors("Pipex: split failed", cmd_split[0], 0);
        exit(EX_CHDPROCFAILURE);  // exit(EXIT_FAILURE);
    }
    // exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
    // cmd_split = cmd_split_arr(cmd);
    
    exe_path = executable_path(cmd_split[0]);
    
    // @ALREADY HANDLED IN get_exe_path
    // if (exe_path == NULL){
    //     perror("cmd1 exe_path NULL");
    //     exit(1);  // exit(EXIT_FAILURE);
    // }

    if(dup2(f1_fd, STDIN_FILENO)==-1){       // setting fd=0 to f1_fd open_file_discription instead terminal/keyboard
        print_errors("Pipex: dup2 failed: ", cmd_split[0], 1);
        exit(EX_FDFAILURE);
    }    
    if(dup2(pipefd[1], STDOUT_FILENO)==-1){ // setting fd=1 to pipe write_end struct_file instead terminal/monitor
        print_errors("Pipex: dup2 failed: ", cmd_split[0], 1);
        exit(EX_FDFAILURE);
    }
    close(f1_fd); // delete original fd
    close(pipefd[1]);  // Write End is successfully copied, delete original fd
    
    execve(exe_path, cmd_split, __environ);
    print_errors("Pipex: execve: ", cmd_split[0], 1);
    free_strings_arr(cmd_split), free(exe_path);
    // }
}

/* 
f2_fd:
    if fails: exit()

exe_path:
    if fails: exit()

close(pipefd[0]):
    pipefd[0] gets -1 on seldom cases where pipe() creat fails on Linux systems
*/
void last_child(const char *cmd, const char *file2, int pipefd[]){
    // pid_t last_child;
    int f2_fd;
    char **cmd_split;
    char *exe_path;
    
    is_file_valid(file2, 0);
    f2_fd = open_file(file2, 0);  // write only
    // l_child = fork();
    // if (l_child == 0){ // Child Process 2 -> cmd2
    close(pipefd[1]);  // No need of Write End
    
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        print_errors("Pipex: split failed", (char*)cmd, 0);
        exit(EX_CHDPROCFAILURE);  // exit(EXIT_FAILURE);
    }
    // exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
    // if (exe_path == NULL){
    //     perror("cmd2 exe_path NULL");
    //     exit(1);  // exit(EXIT_FAILURE);
    // }
    exe_path = executable_path(cmd_split[0]);
  
    dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
    // dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
    dup2(f2_fd, STDOUT_FILENO);
    
    close(f2_fd);  // delete original fd
    close(pipefd[0]);  // Read End is successfully copied, delete it
    
    execve(exe_path, cmd_split, __environ);
    print_errors("Pipex: execve: ", cmd_split[0], 1);
    free_strings_arr(cmd_split);
    free(exe_path);
    // }
}

