/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:43:27 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 07:13:31 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static char	*safe_trim(char *line)
{
	int		start;
	int		end;
	int		len;
	char	*result;
	int		i;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	start = 0;
	while (start < len && (line[start] == ' ' || line[start] == '\t' || 
		line[start] == '\n' || line[start] == '\r'))
		start++;
	end = len - 1;
	while (end >= start && (line[end] == ' ' || line[end] == '\t' || 
		line[end] == '\n' || line[end] == '\r'))
		end--;
	if (start > end)
		return (ft_strdup(""));
	result = malloc(end - start + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (start <= end)
		result[i++] = line[start++];
	result[i] = '\0';
	return (result);
}

static int	is_valid_line(char *line)
{
	char	*trimmed;
	int		result;

	if (!line)
		return (0);
	
	trimmed = safe_trim(line);
	if (!trimmed)
		return (0);
	// Ligne vide = valide
	if (ft_strlen(trimmed) == 0)
	{
		free(trimmed);
		return (1);
	}
	// Vérifier les éléments valides
	result = (ft_strncmp(trimmed, "NO ", 3) == 0 ||
		ft_strncmp(trimmed, "SO ", 3) == 0 ||
		ft_strncmp(trimmed, "WE ", 3) == 0 ||
		ft_strncmp(trimmed, "EA ", 3) == 0 ||
		ft_strncmp(trimmed, "F ", 2) == 0 ||
		ft_strncmp(trimmed, "C ", 2) == 0 ||
		(trimmed[0] == '1' || trimmed[0] == '0' || trimmed[0] == ' '));
	free(trimmed);
	return (result);
}

static int	check_parasites(char **map)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (map[i])
	{
		if (!is_valid_line(map[i]))
		{
			trimmed = ft_strtrim(map[i], " \t\n\r");
			if (trimmed && ft_strlen(trimmed) > 0)
			{
				errormsg("Invalid element found in map file");
				free(trimmed);
				return (-1);
			}
			free(trimmed);
		}
		i++;
	}
	return (0);
}

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
	// Vérifier que le fichier existe et est accessible
	fd = open(texture, O_RDONLY);
	if (fd < 0)
		return (0);
	close(fd);
	return (1);
}

static int	set_texture(char **texture_ptr, char *line, char *error_msg)
{
	char	*trimmed;

	if (!line)
		return (-1);
	
	free(*texture_ptr);
	trimmed = safe_trim(line);  // Utiliser notre version sûre
	*texture_ptr = trimmed;
	if (!is_valid_texture(*texture_ptr))
	{
		errormsg(error_msg);
		return (-1);
	}
	return (0);
}

int	get_textures(t_data *data, char **map)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (ft_strncmp(map[i], "NO ", 3) == 0)
		{
			if (set_texture(&data->no_texture, map[i] + 3, 
					"NO texture missing or not .xpm") == -1)
				return (-1);
		}
		else if (ft_strncmp(map[i], "SO ", 3) == 0)
		{
			if (set_texture(&data->so_texture, map[i] + 3, 
					"SO texture missing or not .xpm") == -1)
				return (-1);
		}
		else if (ft_strncmp(map[i], "WE ", 3) == 0)
		{
			if (set_texture(&data->we_texture, map[i] + 3, 
					"WE texture missing or not .xpm") == -1)
				return (-1);
		}
		else if (ft_strncmp(map[i], "EA ", 3) == 0)
		{
			if (set_texture(&data->ea_texture, map[i] + 3, 
					"EA texture missing or not .xpm") == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	parse_color(char *line, int color[3], char *err)
{
    char	**split;
    int		r, g, b;

    split = ft_split(line, ',');
    if (!split || !split[0] || !split[1] || !split[2] || split[3])
        return (free_tabtab(split), errormsg(err), -1);
    r = ft_atoi(split[0]);
    g = ft_atoi(split[1]);
    b = ft_atoi(split[2]);
    free_tabtab(split);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
        return (errormsg(err), -1);
    color[0] = r;
    color[1] = g;
    color[2] = b;
    return (0);
}

int	get_color(t_data *data)
{
    int	i;

    i = 0;
    while (data->map[i])
    {
        if (ft_strncmp(data->map[i], "F ", 2) == 0 &&
            parse_color(data->map[i] + 2, data->f_color, "Invalid floor color") == -1)
            return (-1);
        if (ft_strncmp(data->map[i], "C ", 2) == 0 &&
            parse_color(data->map[i] + 2, data->c_color, "Invalid ceiling color") == -1)
            return (-1);
        i++;
    }
    return (0);
}

int parsing(t_data *data)
{
    if (check_parasites(data->map) == -1)
        return (-1);
    if (get_textures(data, data->map) == -1)
        return (-1);
    if (!data->no_texture || !data->so_texture || 
		!data->we_texture || !data->ea_texture)
    {
        errormsg("Missing texture definitions in map file");
        return (-1);
    }
    if (get_color(data) == -1)
        return (-1);
    return (0);
}

int iscorrectformat(const char *filename)
{
    size_t len = ft_strlen(filename);

    if (len < 5)
        return 0;
    if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
        return 0;
    return 1;
}
