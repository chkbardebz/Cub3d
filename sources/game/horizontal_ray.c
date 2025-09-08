/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horizontal_ray.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:14:23 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:35:46 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	horizontal_ray_up(t_data *data)
{
	data->ray.hy = (((int)data->player_y >> 6) << 6) - 0.0001;
	data->ray.hx = (data->player_y - data->ray.hy) * data->ray.atan
		+ data->player_x;
	data->ray.yo = -64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(data->ray.hx) >> 6;
		data->ray.my = (int)(data->ray.hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			data->ray.dis_h = dist(data->player_x, data->player_y, data->ray.hx,
					data->ray.hy);
			break ;
		}
		data->ray.hx += data->ray.xo;
		data->ray.hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	horizontal_ray_down(t_data *data)
{
	data->ray.hy = (((int)data->player_y >> 6) << 6) + 64;
	data->ray.hx = (data->player_y - data->ray.hy) * data->ray.atan
		+ data->player_x;
	data->ray.yo = 64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(data->ray.hx) >> 6;
		data->ray.my = (int)(data->ray.hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			data->ray.dis_h = dist(data->player_x, data->player_y, data->ray.hx,
					data->ray.hy);
			break ;
		}
		data->ray.hx += data->ray.xo;
		data->ray.hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	cast_horizontal_ray(t_data *data, double ra)
{
	data->ray.dis_h = 10000;
	data->ray.hx = data->player_x;
	data->ray.hy = data->player_y;
	data->ray.dof = 0;
	data->ray.atan = -1 / tan(ra);
	if (ra > PI)
		horizontal_ray_up(data);
	else if (ra < PI)
		horizontal_ray_down(data);
}
