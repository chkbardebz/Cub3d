/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:36:12 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 03:42:48 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

t_data *get_data(void)
{
    static t_data data;
    return (&data);
}



char **init_map(const char *map_file)
{
    int fd;
    char *line;
    char **map;

    line = NULL;
    fd = open(map_file, O_RDONLY);
    if (fd < 0)
        return NULL; // Handle error
    

    close(fd);
    return map;
}

void init(const char *map_file)
{
    t_data *data;

    data = get_data();
}