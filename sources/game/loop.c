/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:42:25 by judenis           #+#    #+#             */
/*   Updated: 2025/08/18 15:47:12 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"



static void clear_frame(t_data *d, int ceiling, int floorc)
{
	// Remplit le ciel et le sol (deux bandes horizontales)
	for (int y = 0; y < d->w_height; ++y) {
		int col = (y < d->w_height / 2) ? ceiling : floorc;
		for (int x = 0; x < d->w_width; ++x)
			img_put_pixel(&d->frame, x, y, col);
	}
}

void render_frame(void)
{
	t_data *d = get_data();
	clear_frame(d, 0x87CEEB, 0x444444); // ciel/sol
	raycasting();                        // dessine les colonnes dans d->frame
	mlx_put_image_to_window(d->mlx_ptr, d->win_ptr, d->frame.img, 0, 0);
}

int key_hook(int keycode)
{
    t_data *data = get_data();
    printf("X : %d, Y : %d, Angle : %f\n", data->player_x, data->player_y, data->p_orientation);
    if (keycode == 119) // W
    {
        data->player_x += data->pdx;
        data->player_y += data->pdy;
        // data->player_y -= 5;
        // if (data->game_map[(int)(data->player_x + data->dir_x * data->move_speed)][(int)data->player_y] == '0')
        // {
        //     data->player_x += data->dir_x * data->move_speed;
        // }
        // if (data->game_map[(int)data->player_x][(int)(data->player_y + data->dir_y * data->move_speed)] == '0')
        // {
        //     data->player_y += data->dir_y * data->move_speed;
        // }
    }
    else if (keycode == 115) // S
    {   
        data->player_x -= data->pdx;
        data->player_y -= data->pdy;
        // if (data->game_map[(int)(data->player_x - data->dir_x * data->move_speed)][(int)data->player_y] == '0')
        // {
        //     data->player_x -= data->dir_x * data->move_speed;
        // }
        // if (data->game_map[(int)data->player_x][(int)(data->player_y - data->dir_y * data->move_speed)] == '0')
        // {
        //     data->player_y -= data->dir_y * data->move_speed;
        // }
    }
    else if (keycode == 97) // A
    {
        // data->player_x += cos((data->p_orientation - 90) * M_PI / 180) * 0.1;
        // data->player_y += sin((data->p_orientation - 90) * M_PI / 180) * 0.1;
    }
    else if (keycode == 100) // D
    {
        // data->player_x += cos((data->p_orientation + 90) * M_PI / 180) * 0.1;
        // data->player_y += sin((data->p_orientation + 90) * M_PI / 180) * 0.1;
    }
    else if (keycode == 65361) // Left arrow
    {
        data->p_orientation -=0.1;
        if (data->p_orientation > 2* PI)
        {
            data->p_orientation -= 2 * PI;
        }
        data->pdx = cos(data->p_orientation) * 5;
        data->pdy = sin(data->p_orientation) * 5;
    }
    else if (keycode == 65363) // Right arrow
    {
        data->p_orientation +=0.1;
        if (data->p_orientation <0)
        {
            data->p_orientation += 2 * PI;
        }
        data->pdx = cos(data->p_orientation) * 5;
        data->pdy = sin(data->p_orientation) * 5;
    }
    else if (keycode == 65307) // Escape
    {
        exit_game(0);
    }
    clear_frame(data, rgb_to_hex(data->c_color[0], data->c_color[1], data->c_color[2]), rgb_to_hex(data->f_color[0], data->f_color[1], data->f_color[2])); // ciel/sol
    raycasting();
    // mlx_pixel_put(data->mlx_ptr, data->win_ptr, (int)data->player_x + 500, (int)data->player_y + 500, 0xFFFFFF);
    return 0;
}

void game(void)
{
    t_data *data;
    int tex_width, tex_height;

    data = get_data();
    data->w_height = 1080;
    data->w_width = 1920;
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
    mlx_hook(data->win_ptr, 2, 1L<<0, key_hook, NULL);
    mlx_hook(data->win_ptr, 17, 0L, exit_game, NULL);
    // mlx_clear_window(data->mlx_ptr, data->win_ptr);
    mlx_loop(data->mlx_ptr);
}
