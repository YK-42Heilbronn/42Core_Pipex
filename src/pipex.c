/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 14:49:16 by ykonka            #+#    #+#             */
/*   Updated: 2025/12/29 14:23:08 by ykonka           ###   ########.fr       */
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
            ft_printf("wrong number of args");
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
    
    char **cmd_split;
    char *exe_path;

    
    f1_fd = open_file(file1);
    f2_fd = open_file(file2);

    cmd_split = NULL;
    // ft_printf("cmd2: %s\n", cmd2);
    
    if(pipe(pipefd)==-1){
        perror("pipe failed");
    }
    
    child1 = fork();
    if (child1 == 0){ // Child Process 1 -> cmd1
        close(pipefd[0]); // No need of Read End
        
        cmd_split = ft_split(cmd1, ' ');
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
    // char buf;
    // close(pipefd[1]);
    // while(read(pipefd[0], &buf, 1)>0){
    //     write(STDOUT_FILENO, &buf, 1);
    // }
    // close(pipefd[0]);
    
    child2 = fork();
    if (child2 == 0){ // Child Process 2 -> cmd2
        close(pipefd[1]);  // No need of Write End
        
        cmd_split = ft_split(cmd2, ' ');
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

// void first_child(char *cmd, int f1_fd, int *pipefd){
//     pid_t first_child;
//     char **cmd_split;
    
//     first_child = fork();
//     if (first_child == 0){ // Child Process 1 -> cmd1
//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd1_split failed");
//             exit(0);
//         }
//         dup2(f1_fd, STDIN_FILENO);       // setting fd=0 to f1_fd open_file_discription instead terminal/keyboard
//         dup2(pipefd[1], STDOUT_FILENO); // setting fd=1 to pipe write_end struct_file instead terminal/monitor
//         close(pipefd[1]);
//         close(pipefd[0]);
//         // ft_printf("%s --- %s\n", get_env_path(), cmd_split[0]);
//         // ft_printf("%s\n", get_exe_path((const char*)get_env_path(), cmd_split[0]));
//         execve(get_exe_path((const char*)get_env_path(), cmd_split[0]), 
//         cmd_split, __environ);
//     }
//     if (cmd_split){
//         free_strings_arr(cmd_split);
//         cmd_split = NULL;
//     }
// }

// void chained_children(char *cmd, int *pipefd){
//     int child;
//     char **cmd_split;
    
//     child = fork();
//     if (child == 0){ // Child Process 2 -> cmd2
//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd2_split failed");
//             exit(0);
//         }
//         dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
//         dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
//         close(pipefd[0]);
//         // close(pipefd[1]);  // fork -> copies closed pipefd[1]
//         execve(get_exe_path((const char*)get_env_path(), cmd_split[0]), 
//         cmd_split, __environ);
//     }
//     if (cmd_split){
//         free_strings_arr(cmd_split);
//         cmd_split = NULL;
//     }
// }

// void last_child(char *cmd, int f2_fd, int *pipefd){
//     int last_child;
//     char **cmd_split;
    
//     last_child = fork();
//     if (last_child == 0){ // Child Process 2 -> cmd2
//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd2_split failed");
//             exit(0);
//         }
//         dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
//         // dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
//         dup2(f2_fd, STDOUT_FILENO);
//         close(pipefd[0]);
//         // close(pipefd[1]);  // fork -> copies closed pipefd[1]
//         execve(get_exe_path((const char*)get_env_path(), cmd_split[0]), 
//         cmd_split, __environ);
//     }
//     if (cmd_split){
//         free_strings_arr(cmd_split);
//         cmd_split = NULL;
//     }
// }

