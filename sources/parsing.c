/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:51:47 by judenis           #+#    #+#             */
/*   Updated: 2025/08/18 15:17:27 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static char	*safe_trim(char *line)
{
    int		start;
    int		end;
    int		len;
    char	*result;
    int		i;

    if (!line)
        return (NULL);
    len = ft_strlen(line);
    start = 0;
    while (start < len && (line[start] == ' ' || line[start] == '\t' || 
        line[start] == '\n' || line[start] == '\r'))
        start++;
    end = len - 1;
    while (end >= start && (line[end] == ' ' || line[end] == '\t' || 
        line[end] == '\n' || line[end] == '\r'))
        end--;
    if (start > end)
        return (ft_strdup(""));
    result = malloc(end - start + 2);
    if (!result)
        return (NULL);
    i = 0;
    while (start <= end)
        result[i++] = line[start++];
    result[i] = '\0';
    return (result);
}

static int	is_map_line(char *line)
{
    int	i;

    if (!line || ft_strlen(line) == 0)
        return (0);
    
    i = 0;
    while (line[i])
    {
        if (line[i] != '0' && line[i] != '1' && line[i] != ' ' &&
            line[i] != 'N' && line[i] != 'S' && line[i] != 'E' && 
            line[i] != 'W' && line[i] != '\n' && line[i] != '\r')
            return (0);
        i++;
    }
    return (1);
}

static int	is_valid_line(char *line)
{
    char	*trimmed;
    int		result;

    if (!line)
        return (0);
    
    trimmed = safe_trim(line);
    if (!trimmed)
        return (0);

    // Ligne vide = valide
    if (ft_strlen(trimmed) == 0)
    {
        free(trimmed);
        return (1);
    }
    // Vérifier les éléments valides
    result = (ft_strncmp(trimmed, "NO ", 3) == 0 ||
        ft_strncmp(trimmed, "SO ", 3) == 0 ||
        ft_strncmp(trimmed, "WE ", 3) == 0 ||
        ft_strncmp(trimmed, "EA ", 3) == 0 ||
        ft_strncmp(trimmed, "F ", 2) == 0 ||
        ft_strncmp(trimmed, "C ", 2) == 0 ||
        is_map_line(trimmed));
    free(trimmed);
    return (result);
}

static int	check_map_position(char **map)
{
    int	i;
    int	map_started;
    int	found_config_after_map;

    i = 0;
    map_started = 0;
    found_config_after_map = 0;

    while (map[i])
    {
        char *trimmed = safe_trim(map[i]);
        if (!trimmed)
        {
            i++;
            continue;
        }

        // Si c'est une ligne de carte
        if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
        {
            map_started = 1;
        }
        // Si c'est un élément de configuration après le début de la carte
        else if (map_started && (ft_strncmp(trimmed, "NO ", 3) == 0 ||
            ft_strncmp(trimmed, "SO ", 3) == 0 ||
            ft_strncmp(trimmed, "WE ", 3) == 0 ||
            ft_strncmp(trimmed, "EA ", 3) == 0 ||
            ft_strncmp(trimmed, "F ", 2) == 0 ||
            ft_strncmp(trimmed, "C ", 2) == 0))
        {
            found_config_after_map = 1;
            free(trimmed);
            break;
        }
        
        free(trimmed);
        i++;
    }

    if (found_config_after_map)
    {
        errormsg("Map must be at the bottom of the file");
        return (-1);
    }

    return (0);
}

static int	check_elements_before_map(char **map)
{
    int	i;
    int	map_started;
    int	has_f = 0, has_c = 0;
    int	has_no = 0, has_so = 0, has_we = 0, has_ea = 0;

    i = 0;
    map_started = 0;

    while (map[i])
    {
        char *trimmed = safe_trim(map[i]);
        if (!trimmed)
        {
            i++;
            continue;
        }

        // Si c'est une ligne de carte, la carte a commencé
        if (is_map_line(trimmed) && ft_strlen(trimmed) > 0)
        {
            map_started = 1;
            free(trimmed);
            break;
        }

        // Vérifier les éléments de configuration avant la carte
        if (ft_strncmp(trimmed, "NO ", 3) == 0)
            has_no = 1;
        else if (ft_strncmp(trimmed, "SO ", 3) == 0)
            has_so = 1;
        else if (ft_strncmp(trimmed, "WE ", 3) == 0)
            has_we = 1;
        else if (ft_strncmp(trimmed, "EA ", 3) == 0)
            has_ea = 1;
        else if (ft_strncmp(trimmed, "F ", 2) == 0)
            has_f = 1;
        else if (ft_strncmp(trimmed, "C ", 2) == 0)
            has_c = 1;

        free(trimmed);
        i++;
    }

    // Vérifier que tous les éléments requis sont présents avant la carte
    if (!map_started)
    {
        errormsg("No map found in file");
        return (-1);
    }

    if (!has_no || !has_so || !has_we || !has_ea || !has_f || !has_c)
    {
        errormsg("All elements (NO, SO, WE, EA, F, C) must be defined before the map");
        return (-1);
    }

    return (0);
}

static int	check_parasites(char **map)
{
    int		i;
    char	*trimmed;

    i = 0;
    while (map[i])
    {
        if (!is_valid_line(map[i]))
        {
            trimmed = ft_strtrim(map[i], " \t\n\r");
            if (trimmed && ft_strlen(trimmed) > 0)
            {
                errormsg("Invalid element found in map file");
                free(trimmed);
                return (-1);
            }
            free(trimmed);
        }
        i++;
    }
    return (0);
}

static int	is_valid_texture(char *texture)
{
    size_t	len;
    int		fd;

    if (!texture)
        return (0);
    len = ft_strlen(texture);
    if (len < 5)
        return (0);
    if (ft_strncmp(texture + len - 4, ".xpm", 4) != 0)
        return (0);
    // Vérifier que le fichier existe et est accessible
    fd = open(texture, O_RDONLY);
    if (fd < 0)
        return (0);
    close(fd);
    return (1);
}

static int	set_texture(char **texture_ptr, char *line, char *error_msg)
{
    char	*trimmed;

    if (!line || !texture_ptr)
        return (-1);
    
    if (*texture_ptr)
    {
        free(*texture_ptr);
        *texture_ptr = NULL;
    }
    trimmed = safe_trim(line);
	printf("Setting texture: %s\n", trimmed);
    *texture_ptr = trimmed;
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
	int j;
	int Text[4];

	Text[0] = 0; // NO
	Text[1] = 0; // SO
	Text[2] = 0; // WE
	Text[3] = 0; // EA
    i = 0;
	printf("%s\n", map[i]);
    while (map[i])
    {
		j = 0;
		while (map[i][j])
		{
        	if (ft_strncmp(map[i] + j, "WE ", 3) == 0)
        	{
        	    Text[2]++;
        	    if (set_texture(&data->we_texture, map[i] + j + 3, 
        	            "WE texture missing or not .xpm") == -1)
        	        return (-1);
        	}
        	else if (ft_strncmp(map[i] + j, "SO ", 3) == 0)
        	{
        	    Text[1]++;
        	    if (set_texture(&data->so_texture, map[i] + j + 3, 
        	            "SO texture missing or not .xpm") == -1)
        	        return (-1);
        	}
        	else if (ft_strncmp(map[i] + j, "NO ", 3) == 0)
        	{
        	    Text[0]++;
        	    if (set_texture(&data->no_texture, map[i] + j + 3, 
        	            "NO texture missing or not .xpm") == -1)
        	        return (-1);
        	}
        	else if (ft_strncmp(map[i] + j, "EA ", 3) == 0)
        	{
        	    Text[3]++;
        	    if (set_texture(&data->ea_texture, map[i] + j + 3, 
        	            "EA texture missing or not .xpm") == -1)
        	        return (-1);
        	}
			j++;
		}
        i++;
    }
	printf("Textures: NO=%d, SO=%d, WE=%d, EA=%d\n", Text[0], Text[1], Text[2], Text[3]);
    if (Text[0] != 1 || Text[1] != 1 || Text[2] != 1 || Text[3] != 1)
    {
        errormsg("Duplicates/Missing textures");
        return (-1);
    }
    return (0);
}

static int	parse_color(char *line, int color[3], char *err)
{
    char	**split;
    int		r, g, b;

    split = ft_split(line, ',');
    if (!split || !split[0] || !split[1] || !split[2] || split[3])
    {
        if (split)
            free_tabtab(split);
        errormsg(err);
        return (-1);
    }
    r = ft_atoi(split[0]);
    g = ft_atoi(split[1]);
    b = ft_atoi(split[2]);
    free_tabtab(split);
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
    {
        errormsg(err);
        return (-1);
    }
    color[0] = r;
    color[1] = g;
    color[2] = b;
    return (0);
}

int	get_color(t_data *data)
{
    int	i;

    i = 0;
    while (data->map[i])
    {
        if (ft_strncmp(data->map[i], "F ", 2) == 0 &&
            parse_color(data->map[i] + 2, data->f_color, "Invalid floor color") == -1)
            return (-1);
        if (ft_strncmp(data->map[i], "C ", 2) == 0 &&
            parse_color(data->map[i] + 2, data->c_color, "Invalid ceiling color") == -1)
            return (-1);
        i++;
    }
    return (0);
}

int	height_map(char **game_map)
{
    int	height;

    if (!game_map)
        return (0);
    height = 0;
    while (game_map[height])
        height++;
    return (height);
}

int	width_map(char **game_map)
{
    int	max_width;
    int	current_width;
    int	i;

    if (!game_map)
        return (0);
    max_width = 0;
    i = 0;
    while (game_map[i])
    {
        current_width = ft_strlen(game_map[i]);
        // Supprimer le caractère de fin de ligne s'il existe
        if (current_width > 0 && (game_map[i][current_width - 1] == '\n' || 
            game_map[i][current_width - 1] == '\r'))
            current_width--;
        if (current_width > max_width)
            max_width = current_width;
        i++;
    }
    return (max_width);
}

int verif_map(t_data *data)
{
    int height;
    int width;

    if (!data || !data->game_map)
    {
        errormsg("Invalid game map");
        return (-1);
    }
    height = height_map(data->game_map);
    width = width_map(data->game_map);
    
    if (height == 0 || width == 0)
    {
        errormsg("Map is empty");
        return (-1);
    }
    
    printf("Map dimensions: %d x %d (height x width)\n", height, width);
    return (0);
}

int init_orientation(t_data *data)
{
    int i;
    int j;
    int compteur;
    
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
            if (data->game_map[i][j] == 'N')
            {
                data->p_orientation = 3*PI/2;
                data->player_x = j;
                data->player_y = i;
                data->pdx = cos(data->p_orientation) * 5;
                data->pdy = sin(data->p_orientation) * 5;
                compteur++;
            }
            else if (data->game_map[i][j] == 'S')
            {
                data->p_orientation = PI/2;
                data->player_x = j;
                data->player_y = i;
                data->pdx = cos(data->p_orientation) * 5;
                data->pdy = sin(data->p_orientation) * 5;
                compteur++;
            }
            else if (data->game_map[i][j] == 'E')
            {
                data->p_orientation = 0;
                data->player_x = j;
                data->player_y = i;
                data->pdx = cos(data->p_orientation) * 5;
                data->pdy = sin(data->p_orientation) * 5;
                compteur++;
            }
            else if (data->game_map[i][j] == 'W')
            {
                data->p_orientation = PI;
                data->player_x = j;
                data->player_y = i;
                data->pdx = cos(data->p_orientation) * 5;
                data->pdy = sin(data->p_orientation) * 5;
                compteur++;
            }
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

static int	flood_fill_check(char **map, int **visited, int x, int y, int height, int width)
{
    // Vérifier les limites - si on sort de la carte depuis un espace praticable, erreur !
    if (x < 0 || x >= width || y < 0 || y >= height)
        return (-1);
    
    // Si on dépasse la longueur réelle de la ligne
    if (!map[y])
        return (-1);
    
    // Calculer la longueur réelle de la ligne (sans \n)
    int line_len = ft_strlen(map[y]);
    if (line_len > 0 && (map[y][line_len - 1] == '\n' || map[y][line_len - 1] == '\r'))
        line_len--;
    
    if (x >= line_len)
        return (-1);
    
    // Si on a déjà visité cette case
    if (visited[y][x] == 1)
        return (0);
    
    // Si c'est un mur, on s'arrête (OK)
    if (map[y][x] == '1')
        return (0);
    
    // Ignorer les caractères de fin de ligne
    if (map[y][x] == '\n' || map[y][x] == '\r')
        return (0);
    
    // Marquer comme visité
    visited[y][x] = 1;
    
    // Si c'est un espace praticable ou le joueur
    if (map[y][x] == '0' || map[y][x] == 'N' || map[y][x] == 'S' || 
        map[y][x] == 'E' || map[y][x] == 'W')
    {
        // Vérifier les 4 directions
        if (flood_fill_check(map, visited, x + 1, y, height, width) == -1)
            return (-1);
        if (flood_fill_check(map, visited, x - 1, y, height, width) == -1)
            return (-1);
        if (flood_fill_check(map, visited, x, y + 1, height, width) == -1)
            return (-1);
        if (flood_fill_check(map, visited, x, y - 1, height, width) == -1)
            return (-1);
    }
    // Si c'est un espace ' ', on accepte mais on ne continue pas l'exploration
    else if (map[y][x] == ' ')
    {
        return (0);
    }
    // Caractère invalide
    else
    {
        return (-1);
    }
    
    return (0);
}

static int	**create_visited_array(int height, int width)
{
    int	**visited;
    int	i, j;
    
    visited = malloc(sizeof(int *) * height);
    if (!visited)
        return (NULL);
    
    i = 0;
    while (i < height)
    {
        visited[i] = malloc(sizeof(int) * width);
        if (!visited[i])
        {
            while (--i >= 0)
                free(visited[i]);
            free(visited);
            return (NULL);
        }
        j = 0;
        while (j < width)
        {
            visited[i][j] = 0;
            j++;
        }
        i++;
    }
    return (visited);
}

static void	free_visited_array(int **visited, int height)
{
    int	i;
    
    if (!visited)
        return;
    
    i = 0;
    while (i < height)
    {
        free(visited[i]);
        i++;
    }
    free(visited);
}

static void	print_visited_array(int **visited, char **map, int height, int width)
{
    int	i, j;
    int	line_len;

    if (!visited || !map)
        return;

    printf("\n=== VISITED ARRAY DEBUG ===\n");
    printf("Height: %d, Width: %d\n", height, width);
    printf("Legend: 0=not visited, 1=visited, X=out of bounds\n\n");

    i = 0;
    while (i < height && map[i])
    {
        // Calculer la longueur réelle de la ligne (sans \n)
        line_len = ft_strlen(map[i]);
        if (line_len > 0 && (map[i][line_len - 1] == '\n' || map[i][line_len - 1] == '\r'))
            line_len--;

        printf("Line %2d: ", i);
        j = 0;
        while (j < width)
        {
            if (j >= line_len)
                printf("X");  // Hors limites de la ligne
            else
                printf("%d", visited[i][j]);
            j++;
        }
        printf("  |  ");
        
        // Afficher aussi la ligne de map correspondante pour comparaison
        j = 0;
        while (j < line_len && map[i][j] && map[i][j] != '\n' && map[i][j] != '\r')
        {
            printf("%c", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
    printf("=== END VISITED ARRAY ===\n\n");
}

int	check_map_closed(t_data *data)
{
    int	**visited;
    int	height;
    int	width;
    int	result;
    
    if (!data || !data->game_map)
    {
        errormsg("Invalid game map for closed check");
        return (-1);
    }
    
    height = height_map(data->game_map);
    width = width_map(data->game_map);
    
    if (height <= 0 || width <= 0)
    {
        errormsg("Invalid map dimensions");
        return (-1);
    }
    
    visited = create_visited_array(height, width);
    if (!visited)
    {
        errormsg("Memory allocation failed for map check");
        return (-1);
    }
    
    printf("Starting flood fill from player position (%d, %d)\n", data->player_x, data->player_y);
    
    // Lancer le flood fill depuis la position du joueur
    result = flood_fill_check(data->game_map, visited, 
                              data->player_x, data->player_y, height, width);
    
    print_visited_array(visited, data->game_map, height, width);
                            
    free_visited_array(visited, height);
    
    if (result == -1)
    {
        errormsg("Map is not properly closed");
        return (-1);
    }
    
    printf("Map flood fill completed successfully!\n");
    return (0);
}

int parsing(t_data *data)
{
    // 1. Vérifier qu'il n'y a pas d'éléments parasites
    if (check_parasites(data->map) == -1)
        return (-1);

    // 2. Vérifier que tous les éléments sont présents avant la carte
    if (check_elements_before_map(data->map) == -1)
        return (-1);

    // 3. Vérifier que la carte est bien en bas du fichier
    if (check_map_position(data->map) == -1)
        return (-1);

    // 4. Parser les textures
    if (get_textures(data, data->map) == -1)
        return (-1);

    // 5. Vérifier que toutes les textures sont présentes
    if (!data->no_texture || !data->so_texture || !data->we_texture || !data->ea_texture)
    {
        errormsg("Missing texture definitions in map file");
        return (-1);
    }
    
    // 6. Parser les couleurs
    if (get_color(data) == -1)
        return (-1);

    // 7. Vérifier la validité de la carte
    if (verif_map(data) == -1)
        return (-1);

    // 8. Initialiser l'orientation du joueur
    if (init_orientation(data) == -1)
        return (-1);

    printf("Player orientation: %f at (%d, %d)\n", data->p_orientation, data->player_x, data->player_y);

    // 9. Vérifier que la carte est fermée
    if (check_map_closed(data) == -1)
        return (-1);

    data->game_map_int = convert_map_to_int(data);
    if (!data->game_map_int)
    {
        // Gérer l'erreur d'allocation
        errormsg("Erreur lors de la conversion de la carte");
        return (-1);
    }
    
    return (0);
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