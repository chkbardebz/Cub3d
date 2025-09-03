#include "../../includes/cub3d.h"

int	check_collision(t_data *data, double new_x, double new_y)
{
    int	map_x;
    int	map_y;
    int	mp;

    // Convertir les coordonnées du monde en coordonnées de la carte
    map_x = (int)(new_x / 64.0);
    map_y = (int)(new_y / 64.0);
    
    // Vérifier les limites de la carte
    if (map_x < 0 || map_y < 0 || map_x >= data->map_width || map_y >= data->map_height)
        return (1); // Collision (hors limites)
    
    // Vérifier si la case contient un mur
    mp = map_y * data->map_width + map_x;
    if (mp >= 0 && mp < data->map_width * data->map_height)
    {
        if (data->game_map_int[mp] == 1)
            return (1); // Collision avec un mur
    }
    
    return (0); // Pas de collision
}

int	check_collision_with_margin(t_data *data, double new_x, double new_y)
{
    double	margin = 5.0; // Marge autour du joueur
    
    // Vérifier les 4 coins du joueur avec une marge
    if (check_collision(data, new_x - margin, new_y - margin) ||
        check_collision(data, new_x + margin, new_y - margin) ||
        check_collision(data, new_x - margin, new_y + margin) ||
        check_collision(data, new_x + margin, new_y + margin))
        return (1);
    
    return (0);
}