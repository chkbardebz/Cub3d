/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:01:07 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_map_position(char **map)
{
	int		i;
	int		map_started;
	char	*trimmed;

	i = 0;
	map_started = 0;
	while (map[i])
	{
		trimmed = safe_trim(map[i]);
		if (!trimmed)
		{
			i++;
			continue ;
		}
		if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
			map_started = 1;
		else if (map_started && (ft_strncmp(trimmed, "NO ", 3) == 0
				|| ft_strncmp(trimmed, "SO ", 3) == 0
				|| ft_strncmp(trimmed, "WE ", 3) == 0
				|| ft_strncmp(trimmed, "EA ", 3) == 0
				|| ft_strncmp(trimmed, "F ", 2) == 0
				|| ft_strncmp(trimmed, "C ", 2) == 0))
		{
			errormsg("Map must be at the bottom of the file");
			free(trimmed);
			return (-1);
		}
		free(trimmed);
		i++;
	}
	return (0);
}

static int	process_element(char *trimmed, int *has_elements)
{
	if (ft_strncmp(trimmed, "NO ", 3) == 0)
		has_elements[0] = 1;
	else if (ft_strncmp(trimmed, "SO ", 3) == 0)
		has_elements[1] = 1;
	else if (ft_strncmp(trimmed, "WE ", 3) == 0)
		has_elements[2] = 1;
	else if (ft_strncmp(trimmed, "EA ", 3) == 0)
		has_elements[3] = 1;
	else if (ft_strncmp(trimmed, "F ", 2) == 0)
		has_elements[4] = 1;
	else if (ft_strncmp(trimmed, "C ", 2) == 0)
		has_elements[5] = 1;
	return (0);
}

int	check_elements_before_map(char **map)
{
	int		i;
	int		has_elements[6];
	int		map_started;
	char	*trimmed;

	i = 0;
	has_elements[0] = 0;
	has_elements[1] = 0;
	has_elements[2] = 0;
	has_elements[3] = 0;
	has_elements[4] = 0;
	has_elements[5] = 0;
	map_started = 0;
	while (map[i])
	{
		trimmed = safe_trim(map[i]);
		if (!trimmed)
		{
			i++;
			continue ;
		}
		if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
		{
			map_started = 1;
			free(trimmed);
			break ;
		}
		process_element(trimmed, has_elements);
		free(trimmed);
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
