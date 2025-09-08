/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   horizontal_ray.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:14:23 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 17:16:50 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	horizontal_ray_up(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{
	data->ray.dof = 0;
	data->ray.atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) - 0.0001;
	*hx = (data->player_y - *hy) * data->ray.atan + data->player_x;
	data->ray.yo = -64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*hx) >> 6;
		data->ray.my = (int)(*hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += data->ray.xo;
		*hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	horizontal_ray_down(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{
	data->ray.dof = 0;
	data->ray.atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) + 64;
	*hx = (data->player_y - *hy) * data->ray.atan + data->player_x;
	data->ray.yo = 64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*hx) >> 6;
		data->ray.my = (int)(*hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += data->ray.xo;
		*hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	cast_horizontal_ray(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{
	*disH = 10000;
	*hx = data->player_x;
	*hy = data->player_y;
	if (ra > PI)
		horizontal_ray_up(data, ra, disH, hx, hy);
	else if (ra < PI)
		horizontal_ray_down(data, ra, disH, hx, hy);
}
