/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:36:12 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 19:57:55 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	tour2chauffe(const char *filename)
{
	int		fd;
	int		i;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (-1);
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
		i++;
	}
	close(fd);
	return (i);
}

char	**init_map(const char *map_file)
{
	int		fd;
	char	*line;
	char	**map;
	int		i;

	i = tour2chauffe(map_file);
	if (i < 0)
		return (NULL);
	fd = open(map_file, O_RDONLY);
	if (fd < 0)
		return (NULL);
	map = malloc(sizeof(char *) * (i + 1));
	i = 0;
	line = get_next_line(fd);
	while (line)
	{
		map[i++] = line;
		line = get_next_line(fd);
	}
	map[i] = NULL;
	close(fd);
	return (map);
}

static int	is_definite_map_line(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '1' || line[i] == '0')
		return (1);
	return (0);
}

static int	is_potential_map_line_in_section(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (!line[i] || line[i] == '\n' || line[i] == '\r')
		return (1);
	while (line[i] && line[i] != '\n' && line[i] != '\r')
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && line[i] != 'N'
			&& line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
			return (0);
		i++;
	}
	return (1);
}

static int	copy_map_lines(char **dest, char **src, int start, int end)
{
	int	i;
	int	j;

	i = start;
	j = 0;
	while (i <= end)
	{
		dest[j] = ft_strdup(src[i]);
		if (!dest[j])
		{
			while (--j >= 0)
				free(dest[j]);
			free(dest);
			return (-1);
		}
		j++;
		i++;
	}
	dest[j] = NULL;
	return (0);
}

static int	find_map_bounds(char **full_map, int *start, int *end)
{
	int	i;

	*start = -1;
	i = 0;
	while (full_map[i])
	{
		if (is_definite_map_line(full_map[i]))
		{
			*start = i;
			break ;
		}
		i++;
	}
	if (*start == -1)
		return (-1);
	*end = *start;
	while (full_map[i])
	{
		if (is_potential_map_line_in_section(full_map[i]))
			*end = i;
		else
			break ;
		i++;
	}
	return (0);
}

char	**extract_map_only(char **full_map)
{
	int		map_start;
	int		map_end;
	char	**map_only;

	if (!full_map)
		return (NULL);
	if (find_map_bounds(full_map, &map_start, &map_end) == -1)
		return (NULL);
	map_only = malloc(sizeof(char *) * (map_end - map_start + 2));
	if (!map_only)
		return (NULL);
	if (copy_map_lines(map_only, full_map, map_start, map_end) == -1)
		return (NULL);
	return (map_only);
}

void	init_game_settings(t_data *data, const char *map_file)
{
	data->w_height = 720;
	data->w_width = 1280;
	data->filename = ft_strdup(map_file);
	data->no_texture = NULL;
	data->so_texture = NULL;
	data->we_texture = NULL;
	data->ea_texture = NULL;
	data->game_map = NULL;
}

void	init_map_data(t_data *data)
{
	data->map = init_map(data->filename);
	if (!data->map)
	{
		errormsg("Failed to initialize map");
		exit_game(1);
	}
	data->game_map = extract_map_only(data->map);
	if (!data->game_map)
	{
		errormsg("Failed to extract game map");
		exit_game(1);
	}
	data->map_width = width_map(data->game_map);
	data->map_height = height_map(data->game_map);
}

void	init(const char *map_file)
{
	t_data	*data;

	if (!iscorrectformat(map_file))
	{
		errormsg("Map file must have a .cub extension");
		exit_game(1);
	}
	data = get_data();
	init_game_settings(data, map_file);
	init_map_data(data);
	if (parsing(data) == -1)
		exit_game(1);
}
