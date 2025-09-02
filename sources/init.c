/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:36:12 by judenis           #+#    #+#             */
/*   Updated: 2025/09/02 11:35:53 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_data *get_data(void)
{
    static t_data data;
    static int initialized = 0;
    
    if (!initialized)
    {
        ft_memset(&data, 0, sizeof(t_data));
        initialized = 1;
    }
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
    
    // Une fois dans la section carte, toute ligne (même vide) est incluse
    // jusqu'à ce qu'on trouve une ligne qui ne peut clairement pas être de la carte
    
    i = 0;
    while (line[i] && (line[i] == ' ' || line[i] == '\t'))
        i++;
    
    // Ligne vide = potentielle erreur de carte à conserver
    if (!line[i] || line[i] == '\n' || line[i] == '\r')
        return (1);
    
    // Si contient des caractères qui ne peuvent pas être dans une carte
    while (line[i] && line[i] != '\n' && line[i] != '\r')
    {
        if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && 
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && line[i] != 'W')
            return (0); // Clairement pas une ligne de carte
        i++;
    }
    
    return (1); // Potentiellement une ligne de carte
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
    
    // Trouver le début de la carte (première ligne qui commence par 1 ou 0)
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
        return (NULL); // Pas de carte trouvée
    
    // À partir du début, inclure toutes les lignes potentielles jusqu'à la fin logique
    map_end = map_start;
    i = map_start;
    while (full_map[i])
    {
        if (is_potential_map_line_in_section(full_map[i]))
            map_end = i;
        else
            break; // Fin de la section carte
        i++;
    }
    
    // Calculer le nombre de lignes
    map_lines = map_end - map_start + 1;
    
    // Allouer le nouveau tableau
    map_only = malloc(sizeof(char *) * (map_lines + 1));
    if (!map_only)
        return (NULL);
    
    // Copier toutes les lignes de la section carte
    j = 0;
    for (i = map_start; i <= map_end; i++)
    {
        map_only[j] = ft_strdup(full_map[i]);
        if (!map_only[j])
        {
            // Libérer en cas d'erreur
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
    // Initialiser les textures à NULL
    data->no_texture = NULL;
    data->so_texture = NULL;
    data->we_texture = NULL;
    data->ea_texture = NULL;
    data->game_map = NULL;
    if (!(data->map = init_map(map_file))) // A foutre dans une fonction du parsing ou error jsp
    {
        errormsg("Failed to initialize map");
        exit_game(1);
    }
    
    // Extraire seulement la carte AVANT le parsing
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
    
    printf("ceiling color: %d,%d,%d\n", data->c_color[0], data->c_color[1], data->c_color[2]);
    printf("floor color: %d,%d,%d\n", data->f_color[0], data->f_color[1], data->f_color[2]);
    printf("\n=== GAME MAP ONLY ===\n");
    print_tabtab(data->game_map);
    data->dir_x = cos(data->p_orientation);
    data->dir_y = sin(data->p_orientation);
    data->plane_x = -sin(data->p_orientation) * 0.66;
    data->plane_y = cos(data->p_orientation) * 0.66;
}
