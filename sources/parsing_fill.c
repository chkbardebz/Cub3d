/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_fill.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:39:52 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:42:16 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_valid_position(t_data *d, int x, int y)
{
	if (y < 0 || y >= d->map_height || x < 0 || x >= d->map_width)
		return (0);
	if (!d->game_map[y] || x >= (int)ft_strlen(d->game_map[y]))
		return (0);
	return (1);
}

int	flood_fill_recursive(t_data *d, char **v, int x, int y)
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

char	*alloc_line(int width)
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

char	**create_visited_map(t_data *d)
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

void	free_visited_map(char **v, int height)
{
	int	i;

	if (!v)
		return ;
	i = -1;
	while (++i < height)
		free(v[i]);
	free(v);
}
