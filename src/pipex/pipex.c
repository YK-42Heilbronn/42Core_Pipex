/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:49:16 by ykonka            #+#    #+#             */
/*   Updated: 2025/12/30 15:41:59 by ykonka           ###   ########.fr       */
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
            exit(1);
       }else{
            pipex(argv[1], argv[2], argv[3], argv[4]);
       }
    return(1);
}

/* 
Pseudocode:
    while(multi_cmd_executiion>1){
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
*/
void pipex(const char *file1, const char *cmd1, const char *cmd2, const char *file2){
    int f1_fd;
    int f2_fd;
    int pipefd[2];
    
    pid_t child1;
    pid_t child2;
    
    // char **cmd_split;
    
    f1_fd = open_file(file1);
    if (f1_fd == -1){
        perror("file1 open failed");
        exit(1);
    }
    f2_fd = open_file(file2);
    if (f2_fd == -1){
        close(f1_fd);
        perror("file2 open failed");
        exit(1);
    }

    // cmd_split = NULL;
    // ft_printf("cmd2: %s\n", cmd2);
    
    if(pipe(pipefd)==-1){
        close(f1_fd);
        close(f2_fd);
        perror("pipe failed");
        exit(1);  // exit(EXIT_SUCCESS)
    }
    
    child1 = fork();
    if (child1 == -1){
        clean_fds(f1_fd, f2_fd, pipefd);
        perror("child1 failed");
        exit(1);   
    }
    first_child(child1, cmd1, f1_fd, pipefd);
    
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
    
    child2 = fork();
    if (child2 == -1){
        clean_fds(f1_fd, f2_fd, pipefd);
        perror("child1 failed");
        exit(1);   
    }
    last_child(child2, cmd2, f2_fd, pipefd);
    
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
    
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);
    
    // close(pipefd[0]);
    // close(pipefd[1]);
    close(f1_fd);
    close(f2_fd);
   
    exit(0);  // exit(EXIT_SUCCESS);
}

void clean_fds(int f1_fd, int f2_fd, int pipefd[]){
    close(f1_fd);
    close(f2_fd);
    close(pipefd[0]);
    close(pipefd[1]);
}

void first_child(pid_t f_child, const char *cmd, int f1_fd, int pipefd[]){
    // pid_t first_child;
    char **cmd_split;
    char *exe_path;
    
//    f_child = fork();
    if (f_child == 0){ // Child Process 1 -> cmd1
        close(pipefd[0]); // No need of Read End

        cmd_split = ft_split(cmd, ' ');
        if (cmd_split == NULL){
            perror("cmd1_split failed");
            exit(1);  // exit(EXIT_FAILURE);
        }
        
        dup2(f1_fd, STDIN_FILENO);       // setting fd=0 to f1_fd open_file_discription instead terminal/keyboard
        dup2(pipefd[1], STDOUT_FILENO); // setting fd=1 to pipe write_end struct_file instead terminal/monitor
        
        close(f1_fd); // delete original fd
        close(pipefd[1]);  // Write End is successfully copied, delete original fd
        
        exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
        if (exe_path == NULL){
            perror("cmd1 exe_path NULL");
            exit(1);  // exit(EXIT_FAILURE);
        }
        execve(exe_path, cmd_split, __environ);
    }
}



void last_child(pid_t l_child, const char *cmd, int f2_fd, int pipefd[]){
    // pid_t last_child;
    char **cmd_split;
    char *exe_path;
    
    // l_child = fork();
    if (l_child == 0){ // Child Process 2 -> cmd2
        close(pipefd[1]);  // No need of Write End
        
        cmd_split = ft_split(cmd, ' ');
        if (cmd_split == NULL){
            perror("cmd2_split failed");
            exit(1);  // exit(EXIT_FAILURE);
        }
        
        dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
        // dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
        dup2(f2_fd, STDOUT_FILENO);
        
        close(f2_fd);  // delete original fd
        close(pipefd[0]);  // Read End is successfully copied, delete it
        exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
        if (exe_path == NULL){
            perror("cmd2 exe_path NULL");
            exit(1);  // exit(EXIT_FAILURE);
        }
        execve(exe_path, cmd_split, __environ);
    }
}

