/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 14:42:25 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 16:52:34 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	game_loop(t_data *data)
{
	update_movement(data);
	fill_background(data, rgb_to_hex(data->c_color[0], data->c_color[1],
			data->c_color[2]), rgb_to_hex(data->f_color[0], data->f_color[1],
			data->f_color[2]));
	raycasting(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
	return (0);
}

void	init_key_states(t_data *data)
{
	data->key_w = 0;
	data->key_s = 0;
	data->key_a = 0;
	data->key_d = 0;
	data->key_left = 0;
	data->key_right = 0;
}

void	load_addr(t_data *data)
{
	data->no_addr = mlx_get_data_addr(data->no_img, &data->tex_bpp,
			&data->tex_sl, &data->tex_endian);
	data->so_addr = mlx_get_data_addr(data->so_img, &data->tex_bpp,
			&data->tex_sl, &data->tex_endian);
	data->we_addr = mlx_get_data_addr(data->we_img, &data->tex_bpp,
			&data->tex_sl, &data->tex_endian);
	data->ea_addr = mlx_get_data_addr(data->ea_img, &data->tex_bpp,
			&data->tex_sl, &data->tex_endian);
}

int	load_textures(t_data *data)
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
	load_addr(data);
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
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->w_width, data->w_height,
			"Cub3D");
	data->img = mlx_new_image(data->mlx_ptr, data->w_width, data->w_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	mlx_hook(data->win_ptr, 17, 0L, exit_game, NULL);
	mlx_hook(data->win_ptr, 2, 1L << 0, key_press_hook, data);
	mlx_hook(data->win_ptr, 3, 1L << 1, key_release_hook, data);
	mlx_loop_hook(data->mlx_ptr, game_loop, data);
	mlx_loop(data->mlx_ptr);
}
