/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:36:12 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:00:41 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_data *get_data(void)
{
    static t_data data;
    // static int initialized = 0;
    
    // if (!initialized)
    // {
    //     ft_memset(&data, 0, sizeof(t_data));
    //     initialized = 1;
    // }
    return (&data);
}

int tour2chauffe(const char *filename)
{
    int fd;
    int i;
    char *line;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return -1;
    i = 0;
    line = get_next_line(fd);
    while (line)
    {
        free(line);
        line = get_next_line(fd);
        i++;
    }
    close(fd);
    return i;
}

char **init_map(const char *map_file)
{
    int fd;
    char *line;
    char **map;
    int i;
    
    i = tour2chauffe(map_file);
    if (i < 0)
        return NULL;
    fd = open(map_file, O_RDONLY);
    if (fd < 0)
        return NULL;
    map = malloc(sizeof(char *) * (i + 1));
    i = 0;
    line = get_next_line(fd);
    while (line)
    {
        map[i++] = line;
        line = get_next_line(fd);
    }
    map[i] = NULL;
    close(fd);
    return (map);
}

static int is_definite_map_line(char *line)
{
    int i;
    
    if (!line)
        return (0);
    
    // Ignorer les espaces au début
    i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    
    // Si commence par 1 ou 0, c'est définitivement une ligne de carte
    if (line[i] == '1' || line[i] == '0')
        return (1);
        
    return (0);
}

static int is_potential_map_line_in_section(char *line)
{
    int i;
    
    if (!line)
        return (0);
    i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    if (!line[i] || line[i] == '\n' || line[i] == '\r')
        return (1);
    while (line[i] && line[i] != '\n' && line[i] != '\r')
    {
        if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && 
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
            return (0);
        i++;
    }
    return (1);
}

char **extract_map_only(char **full_map)
{
    int i;
    int map_start;
    int map_end;
    int map_lines;
    char **map_only;
    int j;

    if (!full_map)
        return (NULL);
    i = 0;
    map_start = -1;
    while (full_map[i])
    {
        if (is_definite_map_line(full_map[i]))
        {
            map_start = i;
            break;
        }
        i++;
    }
    if (map_start == -1)
        return (NULL);
    map_end = map_start;
    i = map_start;
    while (full_map[i])
    {
        if (is_potential_map_line_in_section(full_map[i]))
            map_end = i;
        else
            break;
        i++;
    }
    map_lines = map_end - map_start + 1;
    map_only = malloc(sizeof(char *) * (map_lines + 1));
    if (!map_only)
        return (NULL);
    j = 0;
    for (i = map_start; i <= map_end; i++)
    {
        map_only[j] = ft_strdup(full_map[i]);
        if (!map_only[j])
        {
            while (--j >= 0)
                free(map_only[j]);
            free(map_only);
            return (NULL);
        }
        j++;
    }
    map_only[j] = NULL;
    
    return (map_only);
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
    data->w_height = 720;
    data->w_width = 1280;
    data->filename = ft_strdup(map_file);
    data->no_texture = NULL;
    data->so_texture = NULL;
    data->we_texture = NULL;
    data->ea_texture = NULL;
    data->game_map = NULL;
    if (!(data->map = init_map(map_file)))
    {
        errormsg("Failed to initialize map");
        exit_game(1);
    }
    data->game_map = extract_map_only(data->map);
    if (!data->game_map)
    {
        errormsg("Failed to extract game map");
        exit_game(1);
    }
    data->map_width = width_map(data->game_map);
    data->map_height = height_map(data->game_map);
    if (parsing(data) == -1)
        exit_game(1);
}
