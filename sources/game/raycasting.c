/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:16:16 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_map_bounds(t_data *data)
{
	int	px;
	int	py;

	if (!data->game_map_int || !data->game_map)
		exit_game(errormsg("game_map_int or game_map is NULL"));
	px = (int)(data->player_x / 64.0);
	py = (int)(data->player_y / 64.0);
	if (data->map_width <= 0 || data->map_height <= 0)
		exit_game(errormsg("Invalid map dimensions"));
	if (px < 0 || py < 0 || px >= data->map_width || py >= data->map_height)
		return (0);
	return (1);
}

void	calculate_wall_params(t_data *data, double ra)
{
	double	ca;
	double	divisor;
	int		half_line;
	int		half_window;

	ca = ra - data->p_orientation;
	data->ray.dis_t = data->ray.dis_t * cos(ca);
	if (data->ray.dis_t > 1.0)
		divisor = data->ray.dis_t;
	else
		divisor = 1.0;
	data->ray.line_h = (int)((64 * data->w_height) / divisor);
	half_window = data->w_height / 2;
	half_line = data->ray.line_h / 2;
	data->ray.line_o = half_window - half_line;
}

int	calculate_texture_x(double rx, double ry, int side, double ra)
{
	double	wall_x;
	int		tex_x;

	if (side == 0)
	{
		wall_x = ry - floor(ry / 64.0) * 64.0;
		tex_x = (int)wall_x;
		if (ra > P2 && ra < P3)
			tex_x = 63 - tex_x;
	}
	else
	{
		wall_x = rx - floor(rx / 64.0) * 64.0;
		tex_x = (int)wall_x;
		if (ra < PI)
			tex_x = 63 - tex_x;
	}
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= 64)
		tex_x = 63;
	return (tex_x);
}

void	process_ray(t_data *data, int r, double ra)
{
	cast_horizontal_ray(data, ra);
	cast_vertical_ray(data, ra);
	if (data->ray.dis_v < data->ray.dis_h)
	{
		data->ray.rx = data->ray.vx;
		data->ray.ry = data->ray.vy;
		data->ray.dis_t = data->ray.dis_v;
		data->ray.side = 0;
	}
	else
	{
		data->ray.rx = data->ray.hx;
		data->ray.ry = data->ray.hy;
		data->ray.dis_t = data->ray.dis_h;
		data->ray.side = 1;
	}
	calculate_wall_params(data, ra);
	data->ray.tex_x = calculate_texture_x(data->ray.rx, data->ray.ry,
			data->ray.side, ra);
	data->ray.texture = select_texture(data, ra, data->ray.side);
	draw_column(data, r, data->ray.texture, data->ray.tex_x);
}

void	raycasting(t_data *data)
{
	double	fov;
	double	dr;
	double	ra;
	int		r;

	fov = 60 * (PI / 180);
	dr = fov / data->w_width;
	r = 0;
	while (r < data->w_width)
	{
		ra = data->p_orientation - (fov / 2) + r * dr;
		if (ra < 0)
			ra += 2 * PI;
		if (ra > 2 * PI)
			ra -= 2 * PI;
		process_ray(data, r, ra);
		r++;
	}
}
