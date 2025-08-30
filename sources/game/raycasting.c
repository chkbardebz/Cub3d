/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/08/30 16:04:15 by judenis          ###   ########.fr       */
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
    int bpp, sl, endian;
    char *addr = mlx_get_data_addr(img, &bpp, &sl, &endian);
    return *(unsigned int *)(addr + (y * sl + x * (bpp / 8)));
}

void my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
    char *dst;
    dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

void raycasting(void)
{
	t_data *data = get_data();
    int px = (int)data->player_x / 64; // conversion pixels → cases
    int py = (int)data->player_y / 64;
    int map_w = ft_strlen(data->game_map[0]);
    int map_h = 0;
    while (data->game_map[map_h])
        map_h++;

    if (px < 0 || py < 0 || px >= map_w || py >= map_h)
    {
        for (int y = 0; y < data->w_height; y++)
            for (int x = 0; x < data->w_width; x++)
                mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, 0x000000);
        return;
    }


	int r , mx , my, mp, dof;
	double rx, ry, ra,xo, yo, disT;
	ra = data->p_orientation - DR * 30;
	if (ra<0)
		ra += 2* PI;
	if (ra>2*PI)
		ra -= 2*PI;
	double fov = 60 * (PI / 180); // FOV de 60 degrés en radians
    double dr = fov / data->w_width; // angle entre chaque rayon

	for (r = 0; r < data->w_width; r++)
	{

		// Calcul correct de l'angle du rayon pour chaque colonne
		ra = data->p_orientation - (fov / 2) + r * dr;
		if (ra < 0)
			ra += 2 * PI;
		if (ra > 2 * PI)
			ra -= 2 * PI;

	// * HORRIZONTAL LINES CHECK * //

		dof = 0;
		double disH = 100000000;
		double hx = data->player_x;
		double hy = data->player_y;
		double aTan = -1/tan(ra);
		if (ra > PI) // FACING UP
		{
			ry = (((int)data->player_y>>6)<<6)-0.0001;
			rx = (data->player_y - ry)*aTan + data->player_x;
			yo = -64;
			xo = -yo*aTan; 
		}
		if (ra < PI) // FACING DOWN
		{
			ry = (((int)data->player_y>>6)<<6) + 64;
			rx = (data->player_y - ry)*aTan + data->player_x;
			yo =64;
			xo =-yo*aTan; 
		}
		if (ra == 0 || ra == PI)
		{
			rx = data->player_x;
			ry = data->player_y;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx)>>6;
			my = (int)(ry)>>6;
			mp = my * data->map_width + mx;
			if (mp >0 && mp <data->map_width * data->map_height&& data->game_map_int[mp]==1)
			{
				hx = rx;
				hy = ry;
				disH=dist(data->player_x, data->player_y, hx,hy);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		
	// * VERTICAL LINES CHECK * //

	
		dof = 0;		
		double disV = 100000000;
		double vx = data->player_x;
		double vy = data->player_y;
		double nTan = -tan(ra);
		if (ra > P2 && ra < P3)
		{
			rx = (((int)data->player_x>>6)<<6)-0.0001;
			ry = (data->player_x - rx)*nTan + data->player_y;
			xo = -64;
			yo = -xo*nTan; 
		}
		if (ra < P2 || ra > P3)
		{
			rx = (((int)data->player_x>>6)<<6) + 64;
			ry = (data->player_x - rx)*nTan + data->player_y;
			xo =64;
			yo =-xo*nTan; 
		}
		if (ra == 0 || ra == PI)
		{
			rx = data->player_x;
			ry = data->player_y;
			dof = 8;
		}
		while (dof < 8)
		{
			mx = (int)(rx)>>6;
			my = (int)(ry)>>6;
			mp = my * data->map_width + mx;
			if (mp > 0 &&mp <data->map_width * data->map_height&& data->game_map_int[mp]==1)
			{
				vx = rx;
				vy = ry;
				disV=dist(data->player_x, data->player_y, vx,vy);
				dof = 8;
			}
			else
			{
				rx += xo;
				ry += yo;
				dof += 1;
			}
		}
		if (disV<disH)
		{
			rx = vx;
			ry = vy;
			disT = disV;
		}
		if (disH<disV)
		{
			rx = hx;
			ry = hy;
			disT = disH;
		}

    // Détermination du mur touché (side et orientation)
    int side;
    double raydirx = cos(ra);
    double raydiry = sin(ra);

    if (disV < disH)
    {
        rx = vx;
        ry = vy;
        disT = disV;
        side = 0; // vertical
    }
    else
    {
        rx = hx;
        ry = hy;
        disT = disH;
        side = 1; // horizontal
    }

    double ca = ra - data->p_orientation;
    disT = disT * cos(ca); // correction du fisheye

    // Calcul de la hauteur de la ligne à dessiner
    int lineH = (int)((64 * data->w_height) / (disT > 0.0001 ? disT : 0.0001));
    if (lineH > data->w_height)
        lineH = data->w_height;
    int lineO = (data->w_height / 2) - (lineH / 2);

    // --- Texture selection ---
    void *texture;
    int tex_width = 64;  // adapte si tes textures ont une autre taille
    int tex_height = 64;

    // Détermination de la direction du mur touché
    if (side == 0) // vertical
    {
        if (raydirx > 0)
            texture = data->ea_img; // Est
        else
            texture = data->we_img; // Ouest
    }
    else // horizontal
    {
        if (raydiry > 0)
            texture = data->so_img; // Sud
        else
            texture = data->no_img; // Nord
    }

    // --- Calcul de la coordonnée X dans la texture ---
    int tex_x;
    if (side == 0)
        tex_x = (int)ry % tex_width;
    else
        tex_x = (int)rx % tex_width;

    // --- Dessin de la colonne avec la texture ---
    for (int y = lineO; y < lineO + lineH; y++)
    {
        int tex_y = ((y - lineO) * tex_height) / lineH;
        int color = get_texture_pixel(texture, tex_x, tex_y);
        my_mlx_pixel_put(data, r, y, color);
    }

		ra +=DR;
		if (ra<0)
			ra += 2* PI;
		if (ra>2*PI)
			ra -= 2*PI;
	}

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
