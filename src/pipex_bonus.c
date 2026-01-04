/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/29 17:43:05 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/03 20:24:02 by ykonka           ###   ########.fr       */
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
*/
// $> < file1 cmd1 | cmd2 > file2
int main(int argc, char *argv[]){
       if (argc < 5){
            // ft_printf("wrong number of args");
            exit(1);
       }else{
            pipex(argc, argv);
       }
    return(1);
}

void pipex(int argc, char **argv){
     char *hd;

     hd = "here_doc";
     if (ft_strncmp(argv[1], hd, ft_strlen(hd))==0){
          here_doc(argc, argv);
     }else{
          multi_cmds(argc, argv);
     }
}

// void clean_fds(int f1_fd, int f2_fd, int pipefd[]){
//     close(f1_fd);
//     close(f2_fd);
//     close(pipefd[0]);
//     close(pipefd[1]);
// }

// void first_child(pid_t f_child, const char *cmd, int f1_fd, int pipefd[]){
//     // pid_t first_child;
//     char **cmd_split;
//     char *exe_path;
    
// //    f_child = fork();
//     if (f_child == 0){ // Child Process 1 -> cmd1
//         close(pipefd[0]); // No need of Read End

//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd1_split failed");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
        
//         dup2(f1_fd, STDIN_FILENO);       // setting fd=0 to f1_fd open_file_discription instead terminal/keyboard
//         dup2(pipefd[1], STDOUT_FILENO); // setting fd=1 to pipe write_end struct_file instead terminal/monitor
        
//         close(f1_fd); // delete original fd
//         close(pipefd[1]);  // Write End is successfully copied, delete original fd
        
//         exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
//         if (exe_path == NULL){
//             perror("cmd1 exe_path NULL");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
//         execve(exe_path, cmd_split, __environ);
//     }
// }



// void last_child(pid_t l_child, const char *cmd, int f2_fd, int pipefd[]){
//     // pid_t last_child;
//     char **cmd_split;
//     char *exe_path;
    
//     // l_child = fork();
//     if (l_child == 0){ // Child Process 2 -> cmd2
//         close(pipefd[1]);  // No need of Write End
        
//         cmd_split = ft_split(cmd, ' ');
//         if (cmd_split == NULL){
//             perror("cmd2_split failed");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
        
//         dup2(pipefd[0], STDIN_FILENO);   // setting fd=0 to pipe read_end struct_file instead f1_fd file
//         // dup2(pipefd[1], STDOUT_FILENO);  // setting fd=1 to pipe write_end struct_file instead terminal/monitor
//         dup2(f2_fd, STDOUT_FILENO);
        
//         close(f2_fd);  // delete original fd
//         close(pipefd[0]);  // Read End is successfully copied, delete it
        
//         exe_path = get_exe_path((const char*)get_env_path(), cmd_split[0]);
//         if (exe_path == NULL){
//             perror("cmd2 exe_path NULL");
//             exit(1);  // exit(EXIT_FAILURE);
//         }
//         execve(exe_path, cmd_split, __environ);
//     }
// }


