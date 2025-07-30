/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:36:12 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 05:48:25 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_data *get_data(void)
{
    static t_data data;
    return (&data);
}

int tour2chauffe(const char *filename)
{
    int fd;
    int i;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return -1; // Handle error
    i = 0;
    line = get_next_line(fd);
    while (line)
    {
        // printf("Line %d: %s", i, line);
        free(line);
        line = get_next_line(fd);
        i++;
    }
    close(fd);
    return i; // Return the number of lines read
}

char **init_map(const char *map_file)
{
    int fd;
    char *line;
    char **map;
    int i;
    
    i = tour2chauffe(map_file);
    if (i < 0)
        return NULL; // Handle error
    fd = open(map_file, O_RDONLY);
    if (fd < 0)
        return NULL; // Handle error
    map = malloc(sizeof(char *) * (i + 1));
    i = 0;
    line = get_next_line(fd);
    while (line)
    {
        map[i++] = line; // Store the line in the map array
        line = get_next_line(fd);
    }
    map[i] = NULL; // Null-terminate the array
    close(fd);
    print_tabtab(map); // Afficher le tableau de la carte
    return (map);
}

void init(const char *map_file)
{
    t_data *data;

    if (!iscorrectformat(map_file))
    {
        errormsg("Invalid map file format");
        exit_game(1);
    }
    data = get_data();
    data->w_height = 1080;
    data->w_width = 1920;
    data->filename = ft_strdup(map_file);
    if (!(data->map = init_map(map_file))) // A foutre dans une fonction du parsing ou error jsp
    {
        errormsg("Failed to initialize map");
        exit_game(1);
    }
    parsing(data);
}
