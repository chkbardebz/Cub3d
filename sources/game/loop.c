/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:42:25 by judenis           #+#    #+#             */
/*   Updated: 2025/08/08 17:24:54 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"



int key_hook(int keycode)
{
    t_data *data = get_data();
    printf("X : %f, Y : %f, Angle : %f\n", data->player_x, data->player_y, data->p_orientation);
    // mlx_pixel_put(data->mlx_ptr, data->win_ptr, (int)data->player_x + 500, (int)data->player_y + 500, 0xFFFFFF);
    if (keycode == 119) // W
    {
        if (data->game_map[(int)(data->player_x + data->dir_x * data->move_speed)][(int)data->player_y] == '0')
        {
            data->player_x += data->dir_x * data->move_speed;
        }
        if (data->game_map[(int)data->player_x][(int)(data->player_y + data->dir_y * data->move_speed)] == '0')
        {
            data->player_y += data->dir_y * data->move_speed;
        }
    }
    else if (keycode == 115) // S
    {
        if (data->game_map[(int)(data->player_x - data->dir_x * data->move_speed)][(int)data->player_y] == '0')
        {
            data->player_x -= data->dir_x * data->move_speed;
        }
        if (data->game_map[(int)data->player_x][(int)(data->player_y - data->dir_y * data->move_speed)] == '0')
        {
            data->player_y -= data->dir_y * data->move_speed;
        }
    }
    else if (keycode == 97) // A
    {
        data->player_x += cos((data->p_orientation - 90) * M_PI / 180) * 0.1;
        data->player_y += sin((data->p_orientation - 90) * M_PI / 180) * 0.1;
    }
    else if (keycode == 100) // D
    {
        data->player_x += cos((data->p_orientation + 90) * M_PI / 180) * 0.1;
        data->player_y += sin((data->p_orientation + 90) * M_PI / 180) * 0.1;
    }
    else if (keycode == 65361) // Left arrow
    {
        double oldDirX = data->dir_x;
        data->dir_x = data->dir_x * cos(-data->rot_speed) - data->dir_y * sin(-data->rot_speed);
        data->dir_y = oldDirX * sin(-data->rot_speed) + data->dir_y * cos(-data->rot_speed);
        double oldPlaneX = data->plane_x;
        data->plane_x = data->plane_x * cos(-data->rot_speed) - data->plane_y * sin(-data->rot_speed);
        data->plane_y = oldPlaneX * sin(-data->rot_speed) + data->plane_y * cos(-data->rot_speed);
        data->p_orientation -= 5;
        if (data->p_orientation < 0)
            data->p_orientation += 360;
    }
    else if (keycode == 65363) // Right arrow
    {
        double oldDirX = data->dir_x;
        data->dir_x = data->dir_x * cos(data->rot_speed) - data->dir_y * sin(data->rot_speed);
        data->dir_y = oldDirX * sin(data->rot_speed) + data->dir_y * cos(data->rot_speed);
        double oldPlaneX = data->plane_x;
        data->plane_x = data->plane_x * cos(data->rot_speed) - data->plane_y * sin(data->rot_speed);
        data->plane_y = oldPlaneX * sin(data->rot_speed) + data->plane_y * cos(data->rot_speed);
        data->p_orientation += 5;
        if (data->p_orientation >= 360)
            data->p_orientation -= 360;
    }
    else if (keycode == 65307) // Escape
    {
        exit_game(0);
    }
    return 0;
}

void game(void)
{
    t_data *data;
    int tex_width, tex_height;

    data = get_data();
    data->w_height = 480;
    data->w_width = 640;
    data->mlx_ptr = mlx_init();
    if (!data->mlx_ptr)
        exit_game(errormsg("Failed to initialize MLX"));
    data->no_img = mlx_xpm_file_to_image(data->mlx_ptr, data->no_texture, &tex_width, &tex_height);
    if (!data->no_img)
        exit_game(errormsg("Failed to load North texture"));
    data->so_img = mlx_xpm_file_to_image(data->mlx_ptr, data->so_texture, &tex_width, &tex_height);
    if (!data->so_img)
        exit_game(errormsg("Failed to load South texture"));
    data->we_img = mlx_xpm_file_to_image(data->mlx_ptr, data->we_texture, &tex_width, &tex_height);
    if (!data->we_img)
        exit_game(errormsg("Failed to load West texture"));
    data->ea_img = mlx_xpm_file_to_image(data->mlx_ptr, data->ea_texture, &tex_width, &tex_height);
    if (!data->ea_img)
        exit_game(errormsg("Failed to load East texture"));
    data->win_ptr = mlx_new_window(data->mlx_ptr, data->w_width, data->w_height, "Cub3D");
    raycasting();
    mlx_hook(data->win_ptr, 2, 1L<<0, key_hook, NULL);
    mlx_hook(data->win_ptr, 17, 0L, exit_game, NULL);
    mlx_clear_window(data->mlx_ptr, data->win_ptr);
    mlx_loop(data->mlx_ptr);
}
