/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:39:29 by judenis           #+#    #+#             */
/*   Updated: 2025/07/31 19:16:06 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void free_tabtab(char **tab)
{
    int i = 0;

    if (!tab)
        return;

    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

int exit_game(int return_value)
{
    t_data *data;

    data = get_data();
    if (data->filename)
        free(data->filename);
    if (data->no_texture)
        free(data->no_texture);
    if (data->so_texture)
        free(data->so_texture);
    if (data->we_texture)
        free(data->we_texture);
    if (data->ea_texture)
        free(data->ea_texture);
    if (data->map)
    {
        free_tabtab(data->map);
        data->map = NULL;
    }
    if (data->game_map)
    {
        free_tabtab(data->game_map);
        data->game_map = NULL;
    }
    if (data->mlx_ptr && data->win_ptr)
    {
        mlx_destroy_window(data->mlx_ptr, data->win_ptr);
        data->win_ptr = NULL;
    }
    exit(return_value);
}
