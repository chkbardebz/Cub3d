/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_conversion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:54:11 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:55:35 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	init_int_map(int *int_map, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		int_map[i] = 0;
		i++;
	}
}

static int	convert_char_to_int(char c)
{
	if (c == '1')
		return (1);
	else if (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W'
		|| c == ' ')
		return (0);
	else
		return (0);
}

static void	convert_map_line(t_data *data, int *int_map, int i)
{
	int	j;
	int	index;

	j = 0;
	while (j < data->map_width && data->game_map[i] && data->game_map[i][j])
	{
		index = i * data->map_width + j;
		int_map[index] = convert_char_to_int(data->game_map[i][j]);
		j++;
	}
}

int	*convert_map_to_int(t_data *data)
{
	int	*int_map;
	int	i;

	if (!data || !data->game_map)
		return (NULL);
	int_map = malloc(sizeof(int) * data->map_width * data->map_height);
	if (!int_map)
		return (NULL);
	init_int_map(int_map, data->map_width * data->map_height);
	i = 0;
	while (i < data->map_height)
	{
		convert_map_line(data, int_map, i);
		i++;
	}
	return (int_map);
}
