/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:29:10 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:30:11 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

int	is_definite_map_line(char *line)
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

int	is_potential_map_line_in_section(char *line)
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

int	copy_map_lines(char **dest, char **src, int start, int end)
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
