/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 19:33:30 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_config_line(char *line)
{
	return (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0
		|| ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0);
}

static int	handle_line(char *line, int *map_started)
{
	char	*trimmed;

	trimmed = safe_trim(line);
	if (!trimmed)
		return (0);
	if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
		*map_started = 1;
	else if (*map_started && is_config_line(trimmed))
	{
		errormsg("Map must be at the bottom of the file");
		free(trimmed);
		return (-1);
	}
	free(trimmed);
	return (0);
}

int	check_map_position(char **map)
{
	int	i;
	int	map_started;

	i = 0;
	map_started = 0;
	while (map[i])
	{
		if (handle_line(map[i], &map_started) == -1)
			return (-1);
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

void	check_loop(char **map, int *has_elements, int *map_started, int i)
{
	char	*trimmed;

	trimmed = safe_trim(map[i]);
	if (!trimmed)
		return ;
	if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
	{
		*map_started = 1;
		free(trimmed);
		return ;
	}
	process_element(trimmed, has_elements);
	free(trimmed);
}

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
