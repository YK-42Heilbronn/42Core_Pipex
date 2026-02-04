/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:01:18 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/04 10:56:03 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

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

static char *get_env_path(char *env_key){
    char **env;
    
    env = __environ;
    while (*env){
        if (ft_strncmp(*env, env_key, ft_strlen(env_key))==0){
            return ft_strdup(*env+5);
        }
        env++;       
    }
    return NULL;
}

static char *join_env_path_to_cmd(char *temp_paths_arr, const char *cmd, char **paths_arr){
    char *path_temp;
    char *path;
    
    path_temp = ft_strjoin(temp_paths_arr, "/");
    if (path_temp == NULL){
        free_strings_arr(paths_arr);
        // paths_arr = NULL; // local copy set to NULL
        // perror("1st strjoin failed");
        return NULL;
    }
    path = ft_strjoin(path_temp, cmd);
    free(path_temp);
    if (path == NULL){
        free_strings_arr(paths_arr);
        // paths_arr = NULL;  // local copy set to NULL
        // perror("2nd strjoin failed");
        return NULL;
    }
    return path;
}

static char *get_exe_path(const char *env_path, const char *cmd){
    char **paths_arr;
    char **temp_paths_arr;
    char *path;
    
    if (env_path == NULL || cmd == NULL || !*cmd)
        return NULL;

    paths_arr = ft_split(env_path, ':');
    temp_paths_arr = paths_arr;
    free((char*)env_path);  // usage is done
    while (*temp_paths_arr){
        path = join_env_path_to_cmd(*temp_paths_arr, cmd, paths_arr);
        // printf("%s\n------------------\n", path);
        if (path==NULL)
            return (free_strings_arr(paths_arr), paths_arr = NULL, NULL);

        if (access(path, F_OK) == 0)
            return (free_strings_arr(paths_arr), paths_arr = NULL, path);
        // else{
        //     perror("cmd is invalid");
        //     exit(1);
        // }
        temp_paths_arr++;
        free(path);
        path=NULL;
    }
    return (free_strings_arr(paths_arr), paths_arr = NULL, NULL);
}

static void cmd_access_check(char *cmd, char **cmd_split, int f_ok, int x_ok){
    if (f_ok){
        if (access(cmd, F_OK) != 0){
            errno = ENOENT;
            print_errors("Pipex_Bonus: ", cmd, 1);
            // free(cleaned_cmd);
            free_strings_arr(cmd_split);
            exit(EX_CMDNF);
        }
    }
    if (x_ok){
        if (access(cmd, X_OK) != 0){
            errno = EACCES;
            print_errors("Pipex_Bonus: ", cmd, 1);
            // free(cleaned_cmd);
            free_strings_arr(cmd_split);
            exit(EX_CMDNX);
        }
    }
}

// @Child-Process-Method
//  || cleaned_cmd[0] == '~'
// if (cleaned_cmd == NULL){
//        print_errors("Pipex_Bonus: command not found", cmd, 0);  // cmd is NULL, write() prints nothing @see: excludes/empty_str.c
char *executable_path(char **cmd_split){
    char *cmd_path;
    char *cmd;
    // char *cleaned_cmd;
    // // char *print_str;
    // cleaned_cmd = ft_strtrim(cmd_split[0], " \t\v\n\r\f");
    // // if (ft_strncmp(cmd, "cat", 3)==0){
    cmd = cmd_split[0];
    // if (cmd == NULL){
    //     print_errors("Pipex_Bonus: command not found: ", cmd_split[0], 0);  // cmd is NULL, write() prints nothing @see: excludes/empty_str.c
    //     free_strings_arr(cmd_split);
    //     exit(EX_CMDNF);
    // }
    // relative path
    if (cmd[0] == '/' || cmd[0] == '.'){
        // if (access(cmd, F_OK) != 0){
        //     errno = ENOENT;
        //     print_errors("Pipex_Bonus: ", cmd, 1);
        //     // free(cleaned_cmd);
        //     free_strings_arr(cmd_split);
        //     exit(EX_CMDNF);
        // }
        // if (access(cmd, X_OK) != 0){
        //     errno = EACCES;
        //     print_errors("Pipex_Bonus: ", cmd, 1);
        //     // free(cleaned_cmd);
        //     free_strings_arr(cmd_split);
        //     exit(EX_CMDNX);
        // }
        cmd_access_check(cmd, cmd_split, 1, 1);
        if ((cmd=ft_strdup(cmd_split[0]))==NULL){
            print_errors("Pipex_Bonus: command not found: ", cmd_split[0], 0);
            free_strings_arr(cmd_split);
            exit(EX_CMDNF);
        }
        return cmd;
    }
    // user home dir
    if (cmd[0] == '~'){
        cmd_path = ft_strjoin(get_env_path("HOME="), cmd++);
        // if (access(cmd_path, F_OK) != 0){
        //     errno = ENOENT;
        //     print_errors("Pipex_Bonus: ", cmd, 1);
        //     // free(cmd_path);
        //     free_strings_arr(cmd_split);
        //     exit(EX_CMDNF);
        // }
        // if (access(cmd_path, X_OK) != 0){
        //     errno = EACCES;
        //     print_errors("Pipex_Bonus: ", cmd, 1);
        //     // free(cmd_path);
        //     free_strings_arr(cmd_split);
        //     exit(EX_CMDNX);
        // }
        cmd_access_check(cmd_path, cmd_split, 1, 1);
        return cmd_path;
    }
    // searching environ
    cmd_path = get_exe_path(get_env_path("PATH="), cmd);
    if (!cmd_path){
        // errno = ENOENT;
        print_errors("Pipex_Bonus: command not found: ", cmd, 0);
        // free(cleaned_cmd);
        free_strings_arr(cmd_split);
        exit(EX_CMDNF);
    }
    else{
        // if (access(cmd_path, X_OK) != 0){
        //     errno = EACCES;
        //     print_errors("Pipex_Bonus: ", cmd, 1);
        //     // free(cleaned_cmd);
        //     free_strings_arr(cmd_split);
        //     exit(EX_CMDNX);
        // }
        // // else
        // //     printf("cmd is executable\n");
        cmd_access_check(cmd_path, cmd_split, 0, 1);
    }
    // free(cleaned_cmd);
    return cmd_path;
    // }else{
    //     return NULL;
    // }
}

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

