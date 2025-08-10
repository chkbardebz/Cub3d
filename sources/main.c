/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:34:22 by judenis           #+#    #+#             */
/*   Updated: 2025/08/10 12:33:24 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void debug_print()
{
    t_data *data = get_data();
    printf("Data values :\n");
    printf("Width: %d\n", data->w_width);
    printf("Height: %d\n", data->w_height);
    printf("Player X: %f\n", data->player_x);
    printf("Player Y: %f\n", data->player_y);
    printf("Player Angle: %f\n", data->p_orientation);
    printf("No Texture: %s\n", data->no_texture);
    printf("So Texture: %s\n", data->so_texture);
    printf("We Texture: %s\n", data->we_texture);
    printf("Ea Texture: %s\n", data->ea_texture);
    printf("Floor Color: %d,%d,%d\n", data->f_color[0], data->f_color[1], data->f_color[2]);
    printf("Ceiling Color: %d,%d,%d\n", data->c_color[0], data->c_color[1], data->c_color[2]);
    printf("Game Map:\n");
    print_tabtab(data->game_map);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return errormsg("Usage: ./cub3d <map_file>");
    init(argv[1]);
    debug_print();
    game();

    
    exit_game(0);
    return (0);
}

//Parsing :

// Attention au flood fill qui regarde pas au nord ouest de la map pour je ne sais quelle raison...
// Initialiser la p_orientation

