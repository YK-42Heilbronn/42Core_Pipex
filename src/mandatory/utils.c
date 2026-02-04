/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:42:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/02/04 12:39:05 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

void print_errors(char *pre_text, char *msg_or_cmd, int _perror){
    char *print_str;

    if (_perror){
        // errno = EACCES;
        print_str = ft_strjoin(pre_text, msg_or_cmd);
        perror(print_str);
        free(print_str);
    }else{
        // pre_text = "Pipex: command not found: ";
        write(2, pre_text, ft_strlen(pre_text));
        write(2, msg_or_cmd, ft_strlen(msg_or_cmd));
        write(2, "\n", 1);
    }
}

// @Child-Process-Method
int open_file(const char *file, int r_or_w){
    int fd;

    if (r_or_w)  // 1-MAX = read
        fd = open(file, O_RDONLY);
    else  // 0 = write
        fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if (fd == -1){  // here errno=is_set_by open() system call function
        print_errors("Pipex: ", (char*)file, 1);
        exit (EX_GENFAILURE);
    }
    return fd;
}

int is_file_exist(const char *path){
    if (access(path, F_OK) == 0){
        return 1; // exist
    }
    return 0; // not exist
}

// @Child-Process-Method
void is_file_valid(const char *in_or_out, int infile){
    if (infile){  // infile
        if (!is_file_exist(in_or_out)){
            errno = ENOENT;
            print_errors("Pipex: ", (char*)in_or_out, 1);
            exit(EX_ITFNF);
        }else{
            if (access(in_or_out, R_OK) != 0){
                errno = EACCES;
                print_errors("Pipex: read: ", (char*)in_or_out, 1);
                exit(EX_ITFNR);
            }
        }
    }else{// outfile
        if (is_file_exist(in_or_out)){
            if (access(in_or_out, W_OK) != 0){
                errno = EACCES;
                // perror("outfile write_permission");
                print_errors("Pipex: write: ", (char*)in_or_out, 1);
                exit(EX_OTFNW);
            }
        } // if it does not exist, will be created and given write access
        // return (EX_SUCCESS);
    }
}

void free_strings_arr(char **str_arr){
    char **temp;
    
    if (str_arr == NULL)
        return;
    
    temp = str_arr;
    while (*temp){
        free(*temp);
        *temp = NULL;
        temp++;
    }
    free(str_arr);
    // str_arr = NULL  // sets only local copy to NULL, so do it after in where this method is invoked
}

