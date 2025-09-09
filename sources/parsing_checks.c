/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_checks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:26:40 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_config_line(char *line)
{
	return (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "SO ", 3) == 0
		|| ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "EA ", 3) == 0
		|| ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "C ", 2) == 0);
}

int	handle_line(char *line, int *map_started)
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
