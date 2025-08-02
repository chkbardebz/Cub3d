/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:22:17 by ylouvel           #+#    #+#             */
/*   Updated: 2025/08/01 21:44:29 by judenis          ###   ########.fr       */
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
    char **map;        // Fichier complet (textures + couleurs + carte)
    char **game_map;   // Seulement la carte (lignes de 0, 1, espaces)
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
    int     p_orientation; // 0 = N, 90 = E, 180 = S, 270 = W
    
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
char **extract_map_only(char **full_map);
int height_map(char **game_map);
int width_map(char **game_map);
int verif_map(t_data *data);

#endif
