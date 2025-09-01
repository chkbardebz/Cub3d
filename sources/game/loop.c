/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:42:25 by judenis           #+#    #+#             */
/*   Updated: 2025/09/01 16:46:56 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"



// static void clear_frame(t_data *d, int ceiling, int floorc)
// {
// 	// Remplit le ciel et le sol (deux bandes horizontales)
// 	for (int y = 0; y < d->w_height; ++y) {
// 		int col = (y < d->w_height / 2) ? ceiling : floorc;
// 		for (int x = 0; x < d->w_width; ++x)
// 			img_put_pixel(&d->frame, x, y, col);
// 	}
// }

// void render_frame(void)
// {
// 	t_data *d = get_data();
// 	clear_frame(d, 0x87CEEB, 0x444444); // ciel/sol
// 	raycasting();                        // dessine les colonnes dans d->frame
// 	mlx_put_image_to_window(d->mlx_ptr, d->win_ptr, d->frame.img, 0, 0);
// }


void	fill_background(t_data *img, int color1, int color2)
{
	int x, y;

	y = 0;
	while (y < img->w_height / 2)
	{
		x = 0;
		while (x < img->w_width)
		{
			my_mlx_pixel_put(img, x, y, color1);
			x++;
		}
		y++;
	}
    while (y < img->w_height)
	{
		x = 0;
		while (x < img->w_width)
		{
			my_mlx_pixel_put(img, x, y, color2);
			x++;
		}
		y++;
	}
}

int key_hook(int keycode)
{
    t_data *data = get_data();
    printf("X : %d, Y : %d, Angle : %f\n", data->player_x, data->player_y, data->p_orientation);
    if (keycode == 119) // W
    {
        data->player_x += cos(data->p_orientation) * MOVE_SPEED;
        data->player_y += sin(data->p_orientation) * MOVE_SPEED;
        // data->player_x += data->pdx;
        // data->player_y += data->pdy;
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
        data->player_x -= cos(data->p_orientation) * MOVE_SPEED;
        data->player_y -= sin(data->p_orientation) * MOVE_SPEED;
        // if (data->game_map[(int)(data->player_x - data->dir_x * data->move_speed)][(int)data->player_y] == '0')
        // {
        //     data->player_x -= data->dir_x * data->move_speed;
        // }
        // if (data->game_map[(int)data->player_x][(int)(data->player_y - data->dir_y * data->move_speed)] == '0')
        // {
        //     data->player_y -= data->dir_y * data->move_speed;
        // }
    }
    else if (keycode == 97) // A (gauche)
    {
        // déplacement perpendiculaire à l'orientation (gauche)
        double strafe_angle = data->p_orientation - PI / 2;
        data->player_x += cos(strafe_angle) * 5;
        data->player_y += sin(strafe_angle) * 5;
    }
    else if (keycode == 100) // D (droite)
    {
        // déplacement perpendiculaire à l'orientation (droite)
        double strafe_angle = data->p_orientation + PI / 2;
        data->player_x += cos(strafe_angle) * 5;
        data->player_y += sin(strafe_angle) * 5;
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

    // Vérification si le joueur est hors de la map
    int px = (int)data->player_x / 64;
    int py = (int)data->player_y / 64;
    int map_w = ft_strlen(data->game_map[0]);
    int map_h = 0;
    while (data->game_map[map_h])
        map_h++;

    if (px < 0 || py < 0 || px >= map_w || py >= map_h)
    {
        // Affiche un écran noir et retourne
        fill_background(data, 0x000000, 0x000000);
        mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
        return 0;
    }

    // Sinon, dessine le background normalement
    fill_background(data, 0x003366, 0xE54222); // bleu foncé
    mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
    raycasting();
    // mlx_pixel_put(data->mlx_ptr, data->win_ptr, (int)data->player_x + 50, (int)data->player_y + 50, 0xFFFFFF);
    return 0;
}

void game(void)
{
    t_data *data;
    int tex_width, tex_height;

    data = get_data();
    data->w_height = 480;
    data->w_width = 720;
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
    if (!data->no_img || !data->so_img || !data->we_img || !data->ea_img)
    {
        printf("Erreur : texture non chargée\n");
        exit(1);
    }
    data->win_ptr = mlx_new_window(data->mlx_ptr, data->w_width, data->w_height, "Cub3D");

    // Crée l'image une seule fois ici :
    data->img = mlx_new_image(data->mlx_ptr, data->w_width, data->w_height);
    data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
                                   &data->line_length, &data->endian);

    mlx_hook(data->win_ptr, 2, 1L<<0, key_hook, NULL);
    mlx_hook(data->win_ptr, 17, 0L, exit_game, NULL);
    mlx_loop(data->mlx_ptr);
}
