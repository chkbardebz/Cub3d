/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertical_ray.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:16:26 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 17:16:47 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	vertical_ray_left(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	data->ray.dof = 0;
	data->ray.ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) - 0.0001;
	*vy = (data->player_x - *vx) * data->ray.ntan + data->player_y;
	data->ray.xo = -64;
	data->ray.yo = -data->ray.xo * data->ray.ntan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*vx) >> 6;
		data->ray.my = (int)(*vy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += data->ray.xo;
		*vy += data->ray.yo;
		data->ray.dof++;
	}
}

void	vertical_ray_right(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	data->ray.dof = 0;
	data->ray.ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) + 64;
	*vy = (data->player_x - *vx) * data->ray.ntan + data->player_y;
	data->ray.xo = 64;
	data->ray.yo = -data->ray.xo * data->ray.ntan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*vx) >> 6;
		data->ray.my = (int)(*vy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += data->ray.xo;
		*vy += data->ray.yo;
		data->ray.dof++;
	}
}

void	cast_vertical_ray(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	*disV = 10000;
	*vx = data->player_x;
	*vy = data->player_y;
	if (ra > P2 && ra < P3)
		vertical_ray_left(data, ra, disV, vx, vy);
	else if (ra < P2 || ra > P3)
		vertical_ray_right(data, ra, disV, vx, vy);
}
