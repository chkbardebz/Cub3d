/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 16:53:41 by judenis           #+#    #+#             */
/*   Updated: 2025/09/10 18:34:25 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	check_collision(t_data *data, double new_x, double new_y)
{
	int	map_x;
	int	map_y;
	int	mp;

	map_x = (int)(new_x / 64.0);
	map_y = (int)(new_y / 64.0);
	if (map_x < 0 || map_y < 0 || map_x >= data->map_width
		|| map_y >= data->map_height)
		return (1);
	mp = map_y * data->map_width + map_x;
	if (mp >= 0 && mp < data->map_width * data->map_height)
	{
		if (data->game_map_int[mp] == 1)
			return (1);
	}
	return (0);
}

int	check_collision_with_margin(t_data *data, double new_x, double new_y)
{
	double	margin;

	margin = 5.0;
	if (check_collision(data, new_x - margin, new_y - margin)
		|| check_collision(data, new_x + margin, new_y - margin)
		|| check_collision(data, new_x - margin, new_y + margin)
		|| check_collision(data, new_x + margin, new_y + margin))
		return (1);
	return (0);
}

void	collision(t_data *data, double new_x, double new_y)
{
	if (!check_collision_with_margin(data, new_x, new_y))
	{
		data->player_x = new_x;
		data->player_y = new_y;
	}
	else
	{
		if (!check_collision_with_margin(data, new_x, data->player_y))
			data->player_x = new_x;
		else if (!check_collision_with_margin(data, data->player_x, new_y))
			data->player_y = new_y;
	}
}
