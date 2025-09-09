/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:51:47 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:05:49 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	verif_text(t_data *data)
{
	if (!data->no_texture || !data->so_texture || !data->we_texture
		|| !data->ea_texture)
	{
		errormsg("Missing texture definitions in map file");
		return (-1);
	}
	return (0);
}

int	parsing(t_data *data)
{
	if (check_parasites(data->map) == -1)
		return (-1);
	if (check_elements_before_map(data->map) == -1)
		return (-1);
	if (check_map_position(data->map) == -1)
		return (-1);
	if (get_textures(data, data->map) == -1)
		return (-1);
	if (verif_text(data) == -1)
		return (-1);
	if (get_color(data) == -1)
		return (-1);
	if (verif_map(data) == -1)
		return (-1);
	if (init_orientation(data) == -1)
		return (-1);
	if (check_map_closed(data) == -1)
		return (-1);
	data->game_map_int = convert_map_to_int(data);
	if (!data->game_map_int)
	{
		errormsg("Erreur lors de la conversion de la carte");
		return (-1);
	}
	return (0);
}

int	iscorrectformat(const char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 5)
		return (0);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}
