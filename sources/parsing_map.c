/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:26:27 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:26:41 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_elements_before_map(char **map)
{
	int	i;
	int	has_elements[6];
	int	map_started;

	i = 0;
	while (i < 6)
		has_elements[i++] = 0;
	map_started = 0;
	while (map[i - 6])
	{
		check_loop(map, has_elements, &map_started, i - 6);
		i++;
	}
	if (!map_started)
	{
		errormsg("No map found in file");
		return (-1);
	}
	if (!has_elements[0] || !has_elements[1] || !has_elements[2]
		|| !has_elements[3] || !has_elements[4] || !has_elements[5])
	{
		errormsg("All elements (NO, SO, WE, EA, F, C) must be defined");
		return (-1);
	}
	return (0);
}

int	verif_map(t_data *data)
{
	int	height;
	int	width;

	if (!data || !data->game_map)
	{
		errormsg("Invalid game map");
		return (-1);
	}
	height = height_map(data->game_map);
	width = width_map(data->game_map);
	if (height == 0 || width == 0)
	{
		errormsg("Map is empty");
		return (-1);
	}
	return (0);
}
