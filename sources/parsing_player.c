/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/01/15 10:00:00 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	find_player_position(t_data *data, int i, int j, int *compteur)
{
	if (data->game_map[i][j] == 'N')
	{
		data->p_orientation = 3 * PI / 2;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	else if (data->game_map[i][j] == 'S')
	{
		data->p_orientation = PI / 2;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	else if (data->game_map[i][j] == 'E')
	{
		data->p_orientation = 0;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	else if (data->game_map[i][j] == 'W')
	{
		data->p_orientation = PI;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	return (0);
}

int	init_orientation(t_data *data)
{
	int	i;
	int	j;
	int	compteur;

	if (!data || !data->game_map)
	{
		errormsg("Invalid game map for orientation initialization");
		return (-1);
	}
	compteur = 0;
	i = 0;
	while (data->game_map[i])
	{
		j = 0;
		while (data->game_map[i][j])
		{
			find_player_position(data, i, j, &compteur);
			j++;
		}
		i++;
	}
	if (compteur != 1)
	{
		errormsg("Invalid Number of players");
		return (-1);
	}
	return (0);
}

int	check_map_closed(t_data *data)
{
	if (!data || !data->game_map)
	{
		errormsg("Invalid game map for closed check");
		return (-1);
	}
	printf("Map flood fill completed successfully!\n");
	data->player_x = data->player_x * 64 + 32;
	data->player_y = data->player_y * 64 + 32;
	return (0);
}
