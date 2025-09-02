/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/02 14:07:30 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_valid_texture(char *texture)
{
	size_t	len;
	int		fd;

	if (!texture)
		return (0);
	len = ft_strlen(texture);
	if (len < 5)
		return (0);
	if (ft_strncmp(texture + len - 4, ".xpm", 4) != 0)
		return (0);
	fd = open(texture, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

static int	set_texture(char **texture_ptr, char *line, char *error_msg)
{
	char	*trimmed;

	if (!line || !texture_ptr)
		return (-1);
	if (*texture_ptr)
	{
		free(*texture_ptr);
		*texture_ptr = NULL;
	}
	trimmed = safe_trim(line);
	printf("Setting texture: %s\n", trimmed);
	*texture_ptr = trimmed;
	if (!is_valid_texture(*texture_ptr))
	{
		errormsg(error_msg);
		return (-1);
	}
	return (0);
}

static int	process_texture_line(t_data *data, char *line, int j, int text[4])
{
	if (ft_strncmp(line + j, "WE ", 3) == 0)
	{
		text[2]++;
		if (set_texture(&data->we_texture, line + j + 3,
				"WE texture missing or not .xpm") == -1)
			return (-1);
	}
	else if (ft_strncmp(line + j, "SO ", 3) == 0)
	{
		text[1]++;
		if (set_texture(&data->so_texture, line + j + 3,
				"SO texture missing or not .xpm") == -1)
			return (-1);
	}
	else if (ft_strncmp(line + j, "NO ", 3) == 0)
	{
		text[0]++;
		if (set_texture(&data->no_texture, line + j + 3,
				"NO texture missing or not .xpm") == -1)
			return (-1);
	}
	else if (ft_strncmp(line + j, "EA ", 3) == 0)
	{
		text[3]++;
		if (set_texture(&data->ea_texture, line + j + 3,
				"EA texture missing or not .xpm") == -1)
			return (-1);
	}
	return (0);
}

int	get_textures(t_data *data, char **map)
{
	int	i;
	int	j;
	int	text[4];

	text[0] = 0;
	text[1] = 0;
	text[2] = 0;
	text[3] = 0;
	i = 0;
	printf("%s\n", map[i]);
	while (map[i])
	{
		j = 0;
		while (map[i][j])
		{
			if (process_texture_line(data, map[i], j, text) == -1)
				return (-1);
			j++;
		}
		i++;
	}
	printf("Textures: NO=%d, SO=%d, WE=%d, EA=%d\n", text[0], text[1], text[2], text[3]);
	if (text[0] != 1 || text[1] != 1 || text[2] != 1 || text[3] != 1)
	{
		errormsg("Duplicates/Missing textures");
		return (-1);
	}
	return (0);
}
