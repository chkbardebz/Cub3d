/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_conversion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:54:11 by judenis           #+#    #+#             */
/*   Updated: 2025/08/18 14:57:01 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * Convertit la carte de caractères 2D en tableau d'entiers 1D
 * '0' ou ' ' -> 0 (espace libre)
 * '1' -> 1 (mur)
 * 'N', 'S', 'E', 'W' -> 0 (position du joueur, traité comme espace libre)
 * 
 * @param data: structure contenant la carte à convertir
 * @return: pointeur vers le nouveau tableau d'entiers, NULL en cas d'erreur
 */
int *convert_map_to_int(t_data *data)
{
	int *int_map;
	int i;
	int j;
	int index;
	
	if (!data || !data->game_map)
		return (NULL);
		
	// Allouer la mémoire pour le tableau 1D
	int_map = malloc(sizeof(int) * data->map_width * data->map_height);
	if (!int_map)
		return (NULL);
		
	// Initialiser le tableau à 0
	i = 0;
	while (i < data->map_width * data->map_height)
	{
		int_map[i] = 0;
		i++;
	}
	
	// Convertir la carte caractère par caractère
	i = 0;
	while (i < data->map_height)
	{
		j = 0;
		while (j < data->map_width && data->game_map[i] && data->game_map[i][j])
		{
			index = i * data->map_width + j;
			
			if (data->game_map[i][j] == '1')
				int_map[index] = 1;
			else if (data->game_map[i][j] == '0' || 
					 data->game_map[i][j] == 'N' || 
					 data->game_map[i][j] == 'S' || 
					 data->game_map[i][j] == 'E' || 
					 data->game_map[i][j] == 'W' ||
					 data->game_map[i][j] == ' ')
				int_map[index] = 0;
			else
				int_map[index] = 0; // Par défaut, traiter comme espace libre
				
			j++;
		}
		i++;
	}
	
	return (int_map);
}
