/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:42:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/05 19:09:44 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

void child_del(void *content) {
    (void)content;  // Do nothing - not malloc'd
}

void end_of_parent_proc(t_list **lst){
    // close(STDIN_FILENO);
    // close(STDOUT_FILENO);
    
    t_list *next;
    next = *lst;
    while(next){
        waitpid((pid_t)(intptr_t)next->content, NULL, 0);
        next = next->next;
    }
    ft_lstclear(lst, child_del);
    // printf("------------child- %d-------------\n", child);
    // while(next){
    //     printf("list child: %d\n", (int)(intptr_t)next->content);
    //     next = next->next;
    // }
    exit(0);  // exit(EXIT_SUCCESS);
}
// int open_file_r(const char *file){
//     int fd;

//     fd = open(file, O_CREAT | O_RDONLY, 0700);
//     if (fd == -1){
//         perror("open_file_r");
//         return -1;
//     }
//     return fd;
// }

// int open_file_w(const char *file){
//     int fd;

//     fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0700);
//     if (fd == -1){
//         perror("open_file_w");
//         return -1;
//     }
//     return fd;
// }

// int open_file_a(const char *file){
//     int fd;

//     fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0700);
//     if (fd == -1){
//         perror("utils_bonus/open_file_a");
//         return -1;
//     }
//     return fd;
// }

// char *get_env_path(){
//     char **env;
//     char *find;
//     char **split;
    
//     env = __environ;
//     while (*env){
//         split = ft_split(*env, '=');
//         if (split==NULL)
//                 return NULL;
//         if (ft_strncmp(split[0], "PATH", ft_strlen(split[0]))==0){  // direct use of 4 as len is not good, using str_len catchs edge cases
//             find = ft_strdup(split[1]);  // split[1] == NULL case is handled in ft_split() itself
//             free_strings_arr(split);
//             split = NULL;
//             return find;
//         }
//         free_strings_arr(split);
//         split = NULL;
//         env++;       
//     }
//     return NULL;
// }

// char *join_env_path_to_cmd(char *temp_paths_arr, const char *cmd, char **paths_arr){
//     char *path;
    
//     path = ft_strjoin(temp_paths_arr, "/");
//     if (path == NULL){
//         free_strings_arr(paths_arr);
//         // paths_arr = NULL; // local copy set to NULL
//         // perror("1st strjoin failed");
//         return NULL;
//     }
//     path = ft_strjoin(path, cmd);
//     if (path == NULL){
//         free_strings_arr(paths_arr);
//         // paths_arr = NULL;  // local copy set to NULL
//         // perror("2nd strjoin failed");
//         return NULL;
//     }
//     return path;
// }

// char *get_exe_path(const char *env_path, const char *cmd){
//     char **paths_arr;
//     char **temp_paths_arr;
//     char *path;
    
//     if (env_path == NULL)
//         return NULL;
    
//     paths_arr = ft_split(env_path, ':');
//     temp_paths_arr = paths_arr;
//     free((char*)env_path);  // usage is done
    
//     while (*temp_paths_arr){
//         path = join_env_path_to_cmd(*temp_paths_arr, cmd, paths_arr);
//         if (path==NULL){
//             paths_arr = NULL;
//             return NULL;
//         }
//         if (access(path, X_OK) == 0){
//             free_strings_arr(paths_arr);
//             paths_arr = NULL;
//             return path;
//         } 
//         temp_paths_arr++;   
//     }
//     free_strings_arr(paths_arr);
//     paths_arr = NULL; // perror("no path found");
//     return NULL;
// }

// void free_strings_arr(char **str_arr){
//     char **temp;
    
//     if (str_arr == NULL)
//         return;
    
//     temp = str_arr;
//     while (*temp){
//         free(*temp);
//         *temp = NULL;
//         temp++;
//     }
//     free(str_arr);
//     // str_arr = NULL  // sets only local copy to NULL, so do it after in where this method is invoked
// }

