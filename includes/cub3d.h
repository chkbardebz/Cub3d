/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:22:17 by ylouvel           #+#    #+#             */
/*   Updated: 2025/07/30 05:53:39 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# define BUFFER_SIZE 42

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "../libft/libft.h"
#include "mlx.h"

typedef struct s_data
{
    char *filename;
    char **map;
    void    *mlx_ptr;
    void    *win_ptr;
    int     w_width;
    int     w_height;
    char    *no_texture;
    char    *so_texture;
    char    *we_texture;
    char    *ea_texture;
    int     f_color[3];
    int     c_color[3];
    int     player_x;
    int     player_y;
    
} t_data;

// Function declarations
int errormsg(const char *message);
void init(const char *map_file);
int exit_game(int return_value);
t_data *get_data(void);
char	*get_next_line(int fd);
void print_tabtab(char **tab);
void free_tabtab(char **tab);
int iscorrectformat(const char *filename);
int parsing(t_data *data);

#endif
