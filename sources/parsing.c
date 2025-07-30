/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:43:27 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 05:50:02 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_valid_texture(char *texture)
{
	size_t	len;

	if (!texture)
		return (0);
	len = ft_strlen(texture);
	if (len < 5)
		return (0);
	if (ft_strncmp(texture + len - 4, ".xpm", 4) != 0)
		return (0);
	return (1);
}

static int	set_texture(char **texture_ptr, char *line, char *error_msg)
{
	free(*texture_ptr);
	*texture_ptr = ft_strdup(line);
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

// int get_res(char **map)
// {
//     t_data *data;
//     int i = 0;
//     int width = 0;
//     int height = 0;

//     data = get_data();
//     while (map[i])
//     {
//         if (ft_strncmp(map[i], "R ", 2) == 0)
//         {
//             char **split = ft_split(map[i] + 2, ' ');
//             if (!split || !split[0] || !split[1] || split[2])
//             {
//                 free_tabtab(split);
//                 errormsg("Invalid resolution format");
//                 return -1;
//             }
//             width = ft_atoi(split[0]);
//             height = ft_atoi(split[1]);
//             if (width <= 0 || height <= 0)
//             {
//                 free_tabtab(split);
//                 errormsg("Resolution values must be positive integers");
//                 return -1;
//             }
//             data->w_width = width;
//             data->w_height = height;
//             free_tabtab(split);
//             break;
//         }
//         i++;
//     }
//     if (width <= 0 || height <= 0)
//     {
//         errormsg("Invalid resolution in map file");
//         return -1;
//     }
//     return 0;
// }

int parsing(t_data *data)
{
    // get_res(data->map);
    get_textures(data, data->map);
    if (!data->no_texture || !data->so_texture || !data->we_texture || !data->ea_texture)
    {
        errormsg("Missing texture definitions in map file");
        return -1;
    }
    return 0;
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
