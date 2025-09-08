/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 19:44:55 by judenis          ###   ########.fr       */
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

static int	is_valid_position(t_data *d, int x, int y)
{
	if (y < 0 || y >= d->map_height || x < 0 || x >= d->map_width)
		return (0);
	if (!d->game_map[y] || x >= (int)ft_strlen(d->game_map[y]))
		return (0);
	return (1);
}

static int	flood_fill_recursive(t_data *d, char **v, int x, int y)
{
	if (!is_valid_position(d, x, y))
		return (-1);
	if (d->game_map[y][x] == '1' || v[y][x] == '1')
		return (0);
	if (d->game_map[y][x] == ' ')
		return (-1);
	v[y][x] = '1';
	if (flood_fill_recursive(d, v, x + 1, y) == -1 || flood_fill_recursive(d, v,
			x - 1, y) == -1 || flood_fill_recursive(d, v, x, y + 1) == -1
		|| flood_fill_recursive(d, v, x, y - 1) == -1)
		return (-1);
	return (0);
}

static char	*alloc_line(int width)
{
	char	*line;
	int		j;

	line = malloc(sizeof(char) * (width + 1));
	if (!line)
		return (NULL);
	j = -1;
	while (++j < width)
		line[j] = '0';
	line[j] = '\0';
	return (line);
}

static char	**create_visited_map(t_data *d)
{
	char	**visited;
	int		i;

	visited = malloc(sizeof(char *) * (d->map_height + 1));
	if (!visited)
		return (NULL);
	i = -1;
	while (++i < d->map_height)
	{
		visited[i] = alloc_line(d->map_width);
		if (!visited[i])
		{
			while (--i >= 0)
				free(visited[i]);
			free(visited);
			return (NULL);
		}
	}
	visited[i] = NULL;
	return (visited);
}

static void	free_visited_map(char **v, int height)
{
	int	i;

	if (!v)
		return ;
	i = -1;
	while (++i < height)
		free(v[i]);
	free(v);
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
