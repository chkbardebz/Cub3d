/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:39:36 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static void	set_player_dir(t_data *data, double angle, int x, int y)
{
	data->p_orientation = angle;
	data->player_x = x;
	data->player_y = y;
	data->pdx = cos(angle) * 5;
	data->pdy = sin(angle) * 5;
}

static int	find_player_position(t_data *data, int i, int j, int *count)
{
	if (data->game_map[i][j] == 'N')
	{
		set_player_dir(data, 3 * PI / 2, j, i);
		(*count)++;
	}
	else if (data->game_map[i][j] == 'S')
	{
		set_player_dir(data, PI / 2, j, i);
		(*count)++;
	}
	else if (data->game_map[i][j] == 'E')
	{
		set_player_dir(data, 0, j, i);
		(*count)++;
	}
	else if (data->game_map[i][j] == 'W')
	{
		set_player_dir(data, PI, j, i);
		(*count)++;
	}
	return (0);
}

int	init_orientation(t_data *data)
{
	int	i;
	int	j;
	int	count;

	if (!data || !data->game_map)
		return (errormsg("Invalid game map for orientation init"), -1);
	count = 0;
	i = -1;
	while (data->game_map[++i])
	{
		j = -1;
		while (data->game_map[i][++j])
			find_player_position(data, i, j, &count);
	}
	if (count != 1)
		return (errormsg("Invalid number of players"), -1);
	return (0);
}

int	check_map_closed(t_data *d)
{
	char	**visited;
	int		result;

	if (!d || !d->game_map)
		return (errormsg("Invalid game map for closed check"), -1);
	visited = create_visited_map(d);
	if (!visited)
		return (errormsg("Failed to alloc memory for flood fill"), -1);
	result = flood_fill_recursive(d, visited, (int)d->player_x,
			(int)d->player_y);
	free_visited_map(visited, d->map_height);
	if (result == -1)
		return (errormsg("Map is not closed - player can reach the edge"), -1);
	d->player_x = d->player_x * 64 + 32;
	d->player_y = d->player_y * 64 + 32;
	return (0);
}
