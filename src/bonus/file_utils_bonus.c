/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:00:56 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/14 17:03:56 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

int open_file_r(const char *file){
    int fd;

    fd = open(file, O_RDONLY);
    if (fd == -1){
        // perror("utils_bonus/open_file_r failed");
        print_errors("Pipex_Bonus: ", (char*)file, 1);
        exit(EX_GENFAILURE);
    }
    return fd;
}

int open_file_w(const char *file){
    int fd;

    fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1){
        // perror("utils_bonus/open_file_w failed");
        print_errors("Pipex_Bonus: ", (char*)file, 1);
        exit(EX_GENFAILURE);
    }
    return fd;
}

int open_file_a(const char *file){
    int fd;

    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
    if (fd == -1){
        // perror("utils_bonus/open_file_a failed");
        print_errors("Pipex_Bonus: ", (char*)file, 1);
        exit(EX_GENFAILURE);
    }
    return fd;
}

int is_file_exist(const char *file){
    if (access(file, F_OK) == 0){
        return 1;
    }
    return 0;
}

// @Child-Process-Method
void is_file_valid(const char* in_or_out, int infile){
    if (infile){ // infile
        if (!is_file_exist(in_or_out)){
            errno = ENOENT;
            print_errors("Pipex_Bonus: ", (char*)in_or_out, 1);
            exit(EX_ITFNF);
        }else{
            if (access(in_or_out, R_OK) != 0){
                errno = EACCES;
                print_errors("Pipex_Bonus: read: ", (char*)in_or_out, 1);
                exit(EX_ITFNR);
            }
        }
    }else{ // outfile
        if (is_file_exist(in_or_out)){
            if (access(in_or_out, W_OK) != 0){
                errno = EACCES;
                print_errors("Pipex_Bonus: write: ", (char*)in_or_out, 1);
                exit(EX_OTFNW);
            }
        }
    }
}

