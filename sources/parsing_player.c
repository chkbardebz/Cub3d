/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_player.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:07:42 by judenis          ###   ########.fr       */
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
	if (data->game_map[i][j] == 'S')
	{
		data->p_orientation = PI / 2;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	if (data->game_map[i][j] == 'E')
	{
		data->p_orientation = 0;
		data->player_x = j;
		data->player_y = i;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
		(*compteur)++;
	}
	if (data->game_map[i][j] == 'W')
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

static int	is_valid_position(t_data *data, int x, int y)
{
    if (y < 0 || y >= data->map_height || x < 0 || x >= data->map_width)
        return (0);
    if (!data->game_map[y] || x >= (int)ft_strlen(data->game_map[y]))
        return (0);
    return (1);
}

static int	flood_fill_recursive(t_data *data, char **visited, int x, int y)
{
    // Vérifier les limites
    if (!is_valid_position(data, x, y))
        return (-1); // Sortie de carte = carte ouverte
    
    // Si c'est un mur ou déjà visité
    if (data->game_map[y][x] == '1' || visited[y][x] == '1')
        return (0);
    
    // Si c'est un espace vide en bordure = carte ouverte
    if (data->game_map[y][x] == ' ')
        return (-1);
    
    // Marquer comme visité
    visited[y][x] = '1';
    
    // Vérifier les 4 directions
    if (flood_fill_recursive(data, visited, x + 1, y) == -1 ||
        flood_fill_recursive(data, visited, x - 1, y) == -1 ||
        flood_fill_recursive(data, visited, x, y + 1) == -1 ||
        flood_fill_recursive(data, visited, x, y - 1) == -1)
        return (-1);
    
    return (0);
}

static char	**create_visited_map(t_data *data)
{
    char	**visited;
    int		i;
    int		j;

    visited = malloc(sizeof(char *) * (data->map_height + 1));
    if (!visited)
        return (NULL);
    
    i = 0;
    while (i < data->map_height)
    {
        visited[i] = malloc(sizeof(char) * (data->map_width + 1));
        if (!visited[i])
        {
            while (--i >= 0)
                free(visited[i]);
            free(visited);
            return (NULL);
        }
        j = 0;
        while (j < data->map_width)
        {
            visited[i][j] = '0';
            j++;
        }
        visited[i][j] = '\0';
        i++;
    }
    visited[i] = NULL;
    return (visited);
}

static void	free_visited_map(char **visited, int height)
{
    int	i;

    if (!visited)
        return ;
    i = 0;
    while (i < height)
    {
        free(visited[i]);
        i++;
    }
    free(visited);
}

int	check_map_closed(t_data *data)
{
    char	**visited;
    int		player_grid_x;
    int		player_grid_y;
    int		result;

    if (!data || !data->game_map)
    {
        errormsg("Invalid game map for closed check");
        return (-1);
    }
    // if (data->map_height == 0)
    // {
    //     while (data->game_map[data->map_height])
    //         data->map_height++;
    // }
    // if (data->map_width == 0)
    // {
    //     int i = 0;
    //     while (data->game_map[i])
    //     {
    //         int len = ft_strlen(data->game_map[i]);
    //         if (len > data->map_width)
    //             data->map_width = len;
    //         i++;
    //     }
    // }
    player_grid_x = (int)data->player_x;
    player_grid_y = (int)data->player_y;
    visited = create_visited_map(data);
    if (!visited)
    {
        errormsg("Failed to allocate memory for flood fill");
        return (-1);
    }
    result = flood_fill_recursive(data, visited, player_grid_x, player_grid_y);
    free_visited_map(visited, data->map_height);
    if (result == -1)
    {
        errormsg("Map is not closed - player can reach the edge");
        return (-1);
    }
    data->player_x = data->player_x * 64 + 32;
    data->player_y = data->player_y * 64 + 32;
    
    return (0);
}
