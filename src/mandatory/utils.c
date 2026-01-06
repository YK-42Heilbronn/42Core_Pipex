/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/27 19:42:30 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/05 14:47:00 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex.h"

int open_file(const char *file, int r_or_w){
    int fd;

    if (r_or_w)  // read
        fd = open(file, O_CREAT | O_RDONLY | O_TRUNC, 0700);
    else
        fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0700);
        
    if (fd == -1){
        perror("open_file");
        return -1;
    }
    return fd;
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

