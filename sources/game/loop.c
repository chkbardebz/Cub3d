/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:42:25 by judenis           #+#    #+#             */
/*   Updated: 2025/09/02 13:16:03 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	fill_background(t_data *img, int color1, int color2)
{
	int	x;
	int	y;

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

void	update_movement(t_data *data)
{
	double	move_x;
	double	move_y;
	double	strafe_angle;

	move_x = 0;
	move_y = 0;
	if (data->key_w)
	{
		move_x += cos(data->p_orientation) * MOVE_SPEED;
		move_y += sin(data->p_orientation) * MOVE_SPEED;
	}
	if (data->key_s)
	{
		move_x -= cos(data->p_orientation) * MOVE_SPEED;
		move_y -= sin(data->p_orientation) * MOVE_SPEED;
	}
	if (data->key_a)
	{
		strafe_angle = data->p_orientation - PI / 2;
		move_x += cos(strafe_angle) * MOVE_SPEED;
		move_y += sin(strafe_angle) * MOVE_SPEED;
	}
	if (data->key_d)
	{
		strafe_angle = data->p_orientation + PI / 2;
		move_x += cos(strafe_angle) * MOVE_SPEED;
		move_y += sin(strafe_angle) * MOVE_SPEED;
	}
	data->player_x += move_x;
	data->player_y += move_y;
	if (data->key_left)
	{
		data->p_orientation -= 0.1;
		if (data->p_orientation < 0)
			data->p_orientation += 2 * PI;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
	}
	if (data->key_right)
	{
		data->p_orientation += 0.1;
		if (data->p_orientation > 2 * PI)
			data->p_orientation -= 2 * PI;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
	}
}

int	key_press_hook(int keycode, t_data *data)
{
	if (keycode == 119)
		data->key_w = 1;
	else if (keycode == 115)
		data->key_s = 1;
	else if (keycode == 97)
		data->key_a = 1;
	else if (keycode == 100)
		data->key_d = 1;
	else if (keycode == 65361)
		data->key_left = 1;
	else if (keycode == 65363)
		data->key_right = 1;
	else if (keycode == 65307)
		exit_game(0);
	return (0);
}

int	key_release_hook(int keycode, t_data *data)
{
	if (keycode == 119)
		data->key_w = 0;
	else if (keycode == 115)
		data->key_s = 0;
	else if (keycode == 97)
		data->key_a = 0;
	else if (keycode == 100)
		data->key_d = 0;
	else if (keycode == 65361)
		data->key_left = 0;
	else if (keycode == 65363)
		data->key_right = 0;
	return (0);
}

int	game_loop(t_data *data)
{
	int	px;
	int	py;
	int	map_w;
	int	map_h;

	update_movement(data);
	printf("X : %.2f, Y : %.2f, Angle : %.6f\n", data->player_x,
		data->player_y, data->p_orientation);
	px = (int)data->player_x / 64;
	py = (int)data->player_y / 64;
	map_w = ft_strlen(data->game_map[0]);
	map_h = 0;
	while (data->game_map[map_h])
		map_h++;
	if (px < 0 || py < 0 || px >= map_w || py >= map_h)
	{
		fill_background(data, 0x000000, 0x000000);
		mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
		return (0);
	}
	fill_background(data, rgb_to_hex(data->c_color[0], data->c_color[1],
			data->c_color[2]), rgb_to_hex(data->f_color[0], data->f_color[1],
			data->f_color[2]));
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
	raycasting();
	return (0);
}

static void	init_key_states(t_data *data)
{
	data->key_w = 0;
	data->key_s = 0;
	data->key_a = 0;
	data->key_d = 0;
	data->key_left = 0;
	data->key_right = 0;
}

static int	load_textures(t_data *data)
{
	int	tex_width;
	int	tex_height;

	data->no_img = mlx_xpm_file_to_image(data->mlx_ptr, data->no_texture,
			&tex_width, &tex_height);
	if (!data->no_img)
		exit_game(errormsg("Failed to load North texture"));
	data->so_img = mlx_xpm_file_to_image(data->mlx_ptr, data->so_texture,
			&tex_width, &tex_height);
	if (!data->so_img)
		exit_game(errormsg("Failed to load South texture"));
	data->we_img = mlx_xpm_file_to_image(data->mlx_ptr, data->we_texture,
			&tex_width, &tex_height);
	if (!data->we_img)
		exit_game(errormsg("Failed to load West texture"));
	data->ea_img = mlx_xpm_file_to_image(data->mlx_ptr, data->ea_texture,
			&tex_width, &tex_height);
	if (!data->ea_img)
		exit_game(errormsg("Failed to load East texture"));
	if (!data->no_img || !data->so_img || !data->we_img || !data->ea_img)
	{
		printf("Erreur : texture non chargée\n");
		exit(1);
	}
	return (0);
}

void	game(void)
{
	t_data	*data;

	data = get_data();
	init_key_states(data);
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		exit_game(errormsg("Failed to initialize MLX"));
	load_textures(data);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->w_width,
			data->w_height, "Cub3D");
	data->img = mlx_new_image(data->mlx_ptr, data->w_width, data->w_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	mlx_hook(data->win_ptr, 2, 1L << 0, key_press_hook, data);
	mlx_hook(data->win_ptr, 3, 1L << 1, key_release_hook, data);
	mlx_hook(data->win_ptr, 17, 0L, exit_game, NULL);
	mlx_loop_hook(data->mlx_ptr, game_loop, data);
	mlx_loop(data->mlx_ptr);
}
