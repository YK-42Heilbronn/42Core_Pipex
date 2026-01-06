/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ykonka <ykonka@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/04 20:00:56 by ykonka            #+#    #+#             */
/*   Updated: 2026/01/06 16:11:47 by ykonka           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "pipex_bonus.h"

int open_file_r(const char *file){
    int fd;

    fd = open(file, O_CREAT | O_RDONLY, 0700);
    if (fd == -1){
        perror("utils_bonus/open_file_r failed");
        return -1;
    }
    return fd;
}

int open_file_w(const char *file){
    int fd;

    fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0700);
    if (fd == -1){
        perror("utils_bonus/open_file_w failed");
        return -1;
    }
    return fd;
}

int open_file_a(const char *file){
    int fd;

    fd = open(file, O_CREAT | O_APPEND | O_WRONLY, 0700);
    if (fd == -1){
        perror("utils_bonus/open_file_a failed");
        return -1;
    }
    return fd;
}

