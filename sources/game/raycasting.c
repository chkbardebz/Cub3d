/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/01 18:16:07 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"



// yo = y offset et pas GURT ;O SYBAU



double dist(double ax, double ay, double bx, double by)
{
	return (sqrt((bx - ax) * (bx-ax) + (by-ay) * (by-ay)));
}

int get_texture_pixel(void *img, int x, int y)
{
    // sécurité : img non nul et coords dans la texture (64x64 attendu)
    if (!img || x < 0 || y < 0 || x >= 64 || y >= 64)
        return 0x000000;
    int bpp, sl, endian;
    char *addr = mlx_get_data_addr(img, &bpp, &sl, &endian);
    if (!addr)
        return 0x000000;
    return *(unsigned int *)(addr + (y * sl + x * (bpp / 8)));
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    if (!data->addr)
    {
        fprintf(stderr, "ERROR: screen addr NULL\n");
        return;
    }
    if (x < 0 || x >= data->w_width || y < 0 || y >= data->w_height)
        return;
    int bytes = data->bits_per_pixel / 8;
    char *dst = data->addr + (y * data->line_length + x * bytes);
    *(unsigned int*)dst = color;
}

void raycasting(void)
{
    t_data *data = get_data();
    
    // Vérification que game_map_int existe
    if (!data->game_map_int || !data->game_map)
    {
        fprintf(stderr, "ERROR: game_map_int or game_map is NULL\n");
        return;
    }
    
    // Vérification limites de map - correction du calcul
    int px = (int)(data->player_x / 64.0);
    int py = (int)(data->player_y / 64.0);
    
    // Calcul correct des dimensions
    int map_w = data->map_width;
    int map_h = data->map_height;
    
    // Vérification si les dimensions sont valides
    if (map_w <= 0 || map_h <= 0)
    {
        fprintf(stderr, "ERROR: Invalid map dimensions\n");
        return;
    }

    if (px < 0 || py < 0 || px >= map_w || py >= map_h)
    {
        // Écran noir si hors map
        for (int y = 0; y < data->w_height; y++)
            for (int x = 0; x < data->w_width; x++)
                my_mlx_pixel_put(data, x, y, 0x000000);
        return;
    }

    double fov = 60 * (PI / 180);
    double dr = fov / data->w_width;

    for (int r = 0; r < data->w_width; r++)
    {
        // Calcul angle pour ce rayon
        double ra = data->p_orientation - (fov / 2) + r * dr;
        if (ra < 0) ra += 2 * PI;
        if (ra > 2 * PI) ra -= 2 * PI;

        // ===== RAYONS HORIZONTAUX =====
        int dof = 0;
        double disH = 1000000;
        double hx = data->player_x, hy = data->player_y;
        double aTan = -1/tan(ra);
        
        if (ra > PI) // Regard vers le haut
        {
            hy = (((int)data->player_y >> 6) << 6) - 0.0001;
            hx = (data->player_y - hy) * aTan + data->player_x;
            double yo = -64;
            double xo = -yo * aTan;
            
            while (dof < 8)
            {
                int mx = (int)(hx) >> 6;
                int my = (int)(hy) >> 6;
                
                // Vérification des limites avant accès au tableau
                if (mx < 0 || my < 0 || mx >= map_w || my >= map_h)
                {
                    dof = 8; // Sortir de la boucle
                    break;
                }
                
                int mp = my * data->map_width + mx;
                if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
                {
                    disH = dist(data->player_x, data->player_y, hx, hy);
                    dof = 8;
                }
                else
                {
                    hx += xo;
                    hy += yo;
                    dof++;
                }
            }
        }
        else if (ra < PI) // Regard vers le bas
        {
            hy = (((int)data->player_y >> 6) << 6) + 64;
            hx = (data->player_y - hy) * aTan + data->player_x;
            double yo = 64;
            double xo = -yo * aTan;
            
            while (dof < 8)
            {
                int mx = (int)(hx) >> 6;
                int my = (int)(hy) >> 6;
                
                // Vérification des limites avant accès au tableau
                if (mx < 0 || my < 0 || mx >= map_w || my >= map_h)
                {
                    dof = 8; // Sortir de la boucle
                    break;
                }
                
                int mp = my * data->map_width + mx;
                if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
                {
                    disH = dist(data->player_x, data->player_y, hx, hy);
                    dof = 8;
                }
                else
                {
                    hx += xo;
                    hy += yo;
                    dof++;
                }
            }
        }

        // ===== RAYONS VERTICAUX =====
        dof = 0;
        double disV = 1000000;
        double vx = data->player_x, vy = data->player_y;
        double nTan = -tan(ra);
        
        if (ra > P2 && ra < P3) // Regard vers la gauche
        {
            vx = (((int)data->player_x >> 6) << 6) - 0.0001;
            vy = (data->player_x - vx) * nTan + data->player_y;
            double xo = -64;
            double yo = -xo * nTan;
            
            while (dof < 8)
            {
                int mx = (int)(vx) >> 6;
                int my = (int)(vy) >> 6;
                
                // Vérification des limites avant accès au tableau
                if (mx < 0 || my < 0 || mx >= map_w || my >= map_h)
                {
                    dof = 8; // Sortir de la boucle
                    break;
                }
                
                int mp = my * data->map_width + mx;
                if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
                {
                    disV = dist(data->player_x, data->player_y, vx, vy);
                    dof = 8;
                }
                else
                {
                    vx += xo;
                    vy += yo;
                    dof++;
                }
            }
        }
        else if (ra < P2 || ra > P3) // Regard vers la droite
        {
            vx = (((int)data->player_x >> 6) << 6) + 64;
            vy = (data->player_x - vx) * nTan + data->player_y;
            double xo = 64;
            double yo = -xo * nTan;
            
            while (dof < 8)
            {
                int mx = (int)(vx) >> 6;
                int my = (int)(vy) >> 6;
                
                // Vérification des limites avant accès au tableau
                if (mx < 0 || my < 0 || mx >= map_w || my >= map_h)
                {
                    dof = 8; // Sortir de la boucle
                    break;
                }
                
                int mp = my * data->map_width + mx;
                if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
                {
                    disV = dist(data->player_x, data->player_y, vx, vy);
                    dof = 8;
                }
                else
                {
                    vx += xo;
                    vy += yo;
                    dof++;
                }
            }
        }

        // ===== SÉLECTION DU MUR LE PLUS PROCHE =====
        double rx, ry, disT;
        int side;
        
        if (disV < disH)
        {
            rx = vx; ry = vy; disT = disV; side = 0; // Mur vertical
        }
        else
        {
            rx = hx; ry = hy; disT = disH; side = 1; // Mur horizontal
        }

        // Correction fisheye
        double ca = ra - data->p_orientation;
        disT = disT * cos(ca);

        // ===== CALCUL DE LA COLONNE ===== (remplace ton bloc)
        int lineH = (int)((64 * data->w_height) / (disT > 0.0001 ? disT : 0.0001));
        // NE PAS limiter lineH - garde la vraie hauteur projetée
        int lineO = (data->w_height / 2) - (lineH / 2);

        // ===== CALCUL DE LA COORDONNÉE TEXTURE ===== (remplace ton bloc)
        double wall_x;
        if (side == 0) // Mur vertical
            wall_x = ry - floor(ry / 64.0) * 64.0;
        else // Mur horizontal
            wall_x = rx - floor(rx / 64.0) * 64.0;
            
        int tex_x = (int)wall_x;
        if (tex_x < 0) tex_x = 0;
        if (tex_x >= 64) tex_x = 63;

        // ===== SÉLECTION DE LA TEXTURE ===== (ajoute ce bloc après le calcul de rx,ry,side)
        void *texture;
        if (side == 0) // Mur vertical
        {
            if (ra > P2 && ra < P3) // Regard vers la gauche = mur est
                texture = data->ea_img;
            else // Regard vers la droite = mur ouest
                texture = data->we_img;
        }
        else // Mur horizontal
        {
            if (ra > PI) // Regard vers le haut = mur sud
                texture = data->so_img;
            else // Regard vers le bas = mur nord
                texture = data->no_img;
        }

        if (!texture) // Sécurité
        {
            texture = data->no_img; // Texture par défaut
        }

        // ===== DESSIN DE LA COLONNE ===== (remplace ton bloc)
        int draw_start = (lineO < 0) ? 0 : lineO;
        int draw_end = (lineO + lineH >= data->w_height) ? data->w_height - 1 : lineO + lineH - 1;

        // Calcul correct du mapping texture
        for (int y = draw_start; y <= draw_end; y++)
        {
            // Calcul de tex_y basé sur la position RÉELLE dans la colonne projetée
            int tex_y = (int)(((y - lineO) * 64) / lineH);
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= 64) tex_y = 63;
            
            int color = get_texture_pixel(texture, tex_x, tex_y);
            my_mlx_pixel_put(data, r, y, color);
        }
    }
    // Dessine une ligne verticale au centre de l'écran
    for (int y = 0; y < data->w_height; y++)
        my_mlx_pixel_put(data, data->w_width / 2, y, 0xFF0000); // Rouge
}









// void raycasting(void)
// {
// 	double		posx; //position x du joueur
// 	double		posy; //position y du joueur
// 	double		dirx; //vecteur de direction (commence à -1 pour N, 1 pour S, 0 sinon)
// 	double		diry; //vecteur de direction (commence à -1 pour W, 1 pour E, 0 sinon)
// 	double		planx; //vecteur du plan (commence à 0.66 pour E, -0.66 pour W, 0 sinon)
// 	double		plany; //vecteur du plan (commence à 0.66 pour N, -0.66 pour S, 0 sinon)
// 	double		raydirx; //calcul de direction x du rayon
// 	double		raydiry; //calcul de direction y du rayon
// 	double		camerax; //point x sur la plan camera : Gauche ecran = -1, milieu = 0, droite = 1
// 	int		mapx; // coordonée x du carré dans lequel est pos
// 	int		mapy; // coordonnée y du carré dans lequel est pos
// 	double		sidedistx; //distance que le rayon parcours jusqu'au premier point d'intersection vertical (=un coté x)
// 	double		sidedisty; //distance que le rayon parcours jusqu'au premier point d'intersection horizontal (= un coté y)
// 	double		deltadistx; //distance que rayon parcours entre chaque point d'intersection vertical
// 	double		deltadisty; //distance que le rayon parcours entre chaque point d'intersection horizontal
// 	int		stepx; // -1 si doit sauter un carre dans direction x negative, 1 dans la direction x positive
// 	int		stepy; // -1 si doit sauter un carre dans la direction y negative, 1 dans la direction y positive
// 	int		hit; // 1 si un mur a ete touche, 0 sinon
// 	int		side; // 0 si c'est un cote x qui est touche (vertical), 1 si un cote y (horizontal)
// 	double		perpwalldist; // distance du joueur au mur
// 	int		lineheight; //hauteur de la ligne a dessiner
// 	int		drawstart; //position de debut ou il faut dessiner
// 	int		drawend; //position de fin ou il faut dessiner
// 	int		x; //permet de parcourir tous les rayons
// }

// double getTicks()
// {
//     return (double)clock() / CLOCKS_PER_SEC * 1000.0; // Retourne en millisecondes
// }

// // Define ColorRGB if not already defined elsewhere
// typedef struct {
//     unsigned char r;
//     unsigned char g;
//     unsigned char b;
// } ColorRGB;

// void raycasting(void)
// {
//     t_data *data = get_data();
//     int x = 0;
//     double time = 0;
//     double oldTime = 0;
//     while (x < data->w_width)
//     {

        
//         double cameraX = 2 * x / (double)data->w_width - 1; //x-coordinate in camera space
//         double rayDirX = data->dir_x + data->plane_x * cameraX;
//         double rayDirY = data->dir_y + data->plane_y * cameraX;

//         int mapX = (int)data->player_x;
//         int mapY = (int)data->player_y;
        
//         double sideDistX;
//         double sideDistY;

//         double deltaDistX = fabs(1 / rayDirX);
//         double deltaDistY = fabs(1 / rayDirY);
//         double perpWallDist;

//         int stepX;
//         int stepY;

//         int hit = 0;
//         int side;

//         if (rayDirX < 0)
//         {
//             stepX = -1;
//             sideDistX = (data->player_x - mapX) * deltaDistX;
//         }
//         else
//         {
//             stepX = 1;
//             sideDistX = (mapX + 1.0 - data->player_x) * deltaDistX;
//         }
//         if (rayDirY < 0)
//         {
//             stepY = -1;
//             sideDistY = (data->player_y - mapY) * deltaDistY;
//         }
//         else
//         {
//             stepY = 1;
//             sideDistY = (mapY + 1.0 - data->player_y) * deltaDistY;
//         }
//         while (hit == 0)
//         {
//             if (sideDistX < sideDistY)
//             {
//                 sideDistX += deltaDistX;
//                 mapX += stepX;
//                 side = 0;
//             }
//             else
//             {
//                 sideDistY += deltaDistY;
//                 mapY += stepY;
//                 side = 1;
//             }
//             if (data->game_map[mapX][mapY] > '0')
//                 hit = 1;
//         }
//         if (side == 0)
//             perpWallDist = (sideDistX - deltaDistX);
//         else
//             perpWallDist = (sideDistY - deltaDistY);
        
//         int lineHeight = (int)(data->w_height / perpWallDist);
//         int drawStart = -lineHeight / 2 + data->w_height / 2;
//         if (drawStart < 0)
//             drawStart = 0;
//         int drawEnd = lineHeight / 2 + data->w_height / 2;
//         if (drawEnd >= data->w_height)
//             drawEnd = data->w_height - 1;

//         ColorRGB color;
//         switch (data->game_map[mapX][mapY])
//         {
//             case '1': color = (ColorRGB){255, 255, 0}; break; // Red for walls
//             case '2': color = (ColorRGB){0, 255, 0}; break; // Green for another type of wall
//             case '3': color = (ColorRGB){0, 0, 255}; break; // Blue for another type of wall
//             case '4': color = (ColorRGB){255, 255, 255}; break; // White for another type of wall
//             default: color = (ColorRGB){255, 255, 0}; break; // Yellow for default
//         }
//         if (side == 1)
//         {
//             color.r /= 2;
//             color.g /= 2;
//             color.b /= 2; // Darken color for side walls
//         }
//         for (int y = drawStart; y < drawEnd; y++)
//         {
//             // Assuming a function put_pixel exists to draw pixels on the screen
//             mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, rgb_to_hex(color.r, color.g, color.b));
//         }
//         x++;
//     }
//     oldTime = time;
//     time = getTicks();
//     double frameTime = (time - oldTime) / 1000.0; // Frame time in seconds
//     data->rot_speed = frameTime * ROT_SPEED_MULTIPLIER; // Adjust rotation speed based on frame time
//     data->move_speed = frameTime * MOVE_SPEED_MULTIPLIER; // Adjust movement speed based on frame time
