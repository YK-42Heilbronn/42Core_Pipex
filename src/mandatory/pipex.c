/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:49:16 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/05 17:11:47 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

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
    pipe()
    fork()

    {
        open()
        access()
        readend()
        writeend()
        execve()
    }

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
int main(int argc, char *argv[]){
       if (argc != 5){
            print_errors("Pipex: Usage: ", "infile \"cmd1 -flag1 -flag2\" \"cmd2 -flag1 -flag2\" outfile", 0);
            exit(EX_WARGS);
       }else{
            pipex(argv[1], argv[2], argv[3], argv[4]);
       }
    return(EX_SUCCESS);
}


void pipex(const char *file1, const char *cmd1, const char *cmd2, const char *file2){
    int pipefd[2];
    int c2status;
    pid_t child1;
    pid_t child2;

    if(pipe(pipefd)==-1){
        print_errors("Pipex: ", "", 1);
        exit(EX_PIPEFAILURE);
    }
    if ((child1=create_child_process(pipefd, cmd1))==0)
        child_process(cmd1, file1, pipefd, 0);
    if ((child2=create_child_process(pipefd, cmd2)==0))
        child_process(cmd2, file2, pipefd, 1);
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(child1, NULL, 0);
    waitpid(child2, &c2status, 0);
    if (WIFEXITED(c2status)){
        exit(WEXITSTATUS(c2status));
    } else if (WIFSIGNALED(c2status)){
        exit(128+WTERMSIG(c2status));
    }else
        exit(WSTOPSIG(c2status));
}

pid_t create_child_process(int pipefd[], const char *cmd){
    pid_t child;

    child = fork();
    if (child == -1){
        close(pipefd[0]), close(pipefd[1]);
        print_errors("Pipex: ", (char*)cmd, 1);
        exit(EX_FORKFAILURE);
    }
    return child;
}

// /*
// f1_fd:
//     if fails: exit()

// exe_path:
//     if fails: exit()

// close(pipefd[1]):
//     pipe() creat failure is very rare and pipefds get -1 case also seldom
// */
// void first_child(const char *cmd, const char *file1, int pipefd[]){
//     char **cmd_split;
//     char *exe_path;
//     int f1_fd;

//     is_file_valid(file1, 1);
//     f1_fd = open_file(file1, 1);
//     close(pipefd[0]);
//     cmd_split = ft_split(cmd, ' ');
//     if (cmd_split == NULL){
//         print_errors("Pipex: ", cmd_split[0], 0);
//         exit(EX_CHDPROCFAILURE);
//     }
//     exe_path = executable_path(cmd_split);
//     // if(dup2(f1_fd, STDIN_FILENO)==-1){
//     //     print_errors("Pipex: ", cmd_split[0], 1);
//     //     exit(EX_FDFAILURE);
//     // }
//     // if(dup2(pipefd[1], STDOUT_FILENO)==-1){
//     //     print_errors("Pipex: ", cmd_split[0], 1);
//     //     exit(EX_FDFAILURE);
//     // }
//     // close(f1_fd);
//     // close(pipefd[1]);
//     execve(exe_path, cmd_split, __environ);
//     print_errors("Pipex: ", cmd_split[0], 1);
//     free_strings_arr(cmd_split);
//     free(exe_path);
//     exit(EX_APPFAILURE);
// }

// /*
// f2_fd:
//     if fails: exit()

// exe_path:
//     if fails: exit()

// close(pipefd[0]):
//     pipefd[0] gets -1 on seldom cases where pipe() creat fails on Linux systems
// */
// void last_child(const char *cmd, const char *file2, int pipefd[]){
//     int f2_fd;
//     char **cmd_split;
//     char *exe_path;

//     is_file_valid(file2, 0);
//     f2_fd = open_file(file2, 0);
//     close(pipefd[1]);

//     cmd_split = ft_split(cmd, ' ');
//     if (cmd_split == NULL){
//         print_errors("Pipex: ", (char*)cmd, 0);
//         exit(EX_CHDPROCFAILURE);
//     }
//     exe_path = executable_path(cmd_split);

//     if(dup2(pipefd[0], STDIN_FILENO)==-1){
//         print_errors("Pipex: ", cmd_split[0], 1);
//         exit(EX_FDFAILURE);
//     }
//     if(dup2(f2_fd, STDOUT_FILENO)==-1){
//         print_errors("Pipex: ", cmd_split[0], 1);
//         exit(EX_FDFAILURE);
//     }

//     close(f2_fd);
//     close(pipefd[0]);

//     execve(exe_path, cmd_split, __environ);
//     print_errors("Pipex: ", cmd_split[0], 1);
//     free_strings_arr(cmd_split);
//     free(exe_path);
//     exit(EX_APPFAILURE);
// }

static void child_process_fd_setup(int not_used, int stdin, int stdout, char **cmd_split){
    close(not_used);

    if(dup2(stdin, STDIN_FILENO)==-1){
        print_errors("Pipex: ", cmd_split[0], 1);
        exit(EX_FDFAILURE);
    }
    if(dup2(stdout, STDOUT_FILENO)==-1){
        print_errors("Pipex: ", cmd_split[0], 1);
        exit(EX_FDFAILURE);
    }
    close(stdin);
    close(stdout);
}

/*
child==0
    first_child_process
child==1
    last_child_process
*/
void child_process(const char *cmd, const char *file, int pipefd[], int child){
    char **cmd_split;
    char *exe_path;
    int f_fd;

    is_file_valid(file, child);
    f_fd=open_file(file, child);
    cmd_split = ft_split(cmd, ' ');
    if (cmd_split == NULL){
        print_errors("Pipex: ", cmd_split[0], 0);
        exit(EX_CHDPROCFAILURE);
    }
    exe_path = executable_path(cmd_split);
    if (child==0)
        child_process_fd_setup(pipefd[0], f_fd, pipefd[1], cmd_split);
    if (child==1)
        child_process_fd_setup(pipefd[1], pipefd[0], f_fd, cmd_split);

    execve(exe_path, cmd_split, __environ);
    print_errors("Pipex: ", cmd_split[0], 1);
    free_strings_arr(cmd_split);
    free(exe_path);
    exit(EX_APPFAILURE);
}

