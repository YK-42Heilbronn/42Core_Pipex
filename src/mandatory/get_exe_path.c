/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exe_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/05 14:46:43 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/05 14:54:40 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

char *get_env_path(){
    char **env;
    char *find;
    char **split;
    
    env = __environ;
    while (*env){
        split = ft_split(*env, '=');
        if (split==NULL)
                return NULL;
        if (ft_strncmp(split[0], "PATH", ft_strlen(split[0]))==0){  // direct use of 4 as len is not good, using str_len catchs edge cases
            find = ft_strdup(split[1]);  // split[1] == NULL case is handled in ft_split() itself
            free_strings_arr(split);
            split = NULL;
            return find;
        }
        free_strings_arr(split);
        split = NULL;
        env++;       
    }
    return NULL;
}

char *join_env_path_to_cmd(char *temp_paths_arr, const char *cmd, char **paths_arr){
    char *path;
    
    path = ft_strjoin(temp_paths_arr, "/");
    if (path == NULL){
        free_strings_arr(paths_arr);
        // paths_arr = NULL; // local copy set to NULL
        // perror("1st strjoin failed");
        return NULL;
    }
    path = ft_strjoin(path, cmd);
    if (path == NULL){
        free_strings_arr(paths_arr);
        // paths_arr = NULL;  // local copy set to NULL
        // perror("2nd strjoin failed");
        return NULL;
    }
    return path;
}

char *get_exe_path(const char *env_path, const char *cmd){
    char **paths_arr;
    char **temp_paths_arr;
    char *path;
    
    if (env_path == NULL)
        return NULL;
    
    paths_arr = ft_split(env_path, ':');
    temp_paths_arr = paths_arr;
    free((char*)env_path);  // usage is done
    
    while (*temp_paths_arr){
        path = join_env_path_to_cmd(*temp_paths_arr, cmd, paths_arr);
        if (path==NULL)
            return (paths_arr = NULL, NULL);

        if (access(path, X_OK) == 0)
            return (free_strings_arr(paths_arr), paths_arr = NULL, path);

        temp_paths_arr++;   
    }// free_strings_arr(paths_arr);perror("no path found");
    return (free_strings_arr(paths_arr), paths_arr = NULL, NULL);
}
