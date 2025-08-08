/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/08/08 17:29:02 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

#include <time.h>

#define ROT_SPEED_MULTIPLIER 3.0

#define MOVE_SPEED_MULTIPLIER 5.0

double getTicks()
{
    return (double)clock() / CLOCKS_PER_SEC * 1000.0; // Retourne en millisecondes
}

// Define ColorRGB if not already defined elsewhere
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} ColorRGB;

void raycasting(void)
{
    t_data *data = get_data();
    int x = 0;
    double time = 0;
    double oldTime = 0;
    while (x < data->w_width)
    {

        
        double cameraX = 2 * x / (double)data->w_width - 1; //x-coordinate in camera space
        double rayDirX = data->dir_x + data->plane_x * cameraX;
        double rayDirY = data->dir_y + data->plane_y * cameraX;

        int mapX = (int)data->player_x;
        int mapY = (int)data->player_y;
        
        double sideDistX;
        double sideDistY;

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (data->player_x - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - data->player_x) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (data->player_y - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - data->player_y) * deltaDistY;
        }
        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (data->game_map[mapX][mapY] > '0')
                hit = 1;
        }
        if (side == 0)
            perpWallDist = (sideDistX - deltaDistX);
        else
            perpWallDist = (sideDistY - deltaDistY);
        
        int lineHeight = (int)(data->w_height / perpWallDist);
        int drawStart = -lineHeight / 2 + data->w_height / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + data->w_height / 2;
        if (drawEnd >= data->w_height)
            drawEnd = data->w_height - 1;

        ColorRGB color;
        switch (data->game_map[mapX][mapY])
        {
            case '1': color = (ColorRGB){255, 255, 0}; break; // Red for walls
            case '2': color = (ColorRGB){0, 255, 0}; break; // Green for another type of wall
            case '3': color = (ColorRGB){0, 0, 255}; break; // Blue for another type of wall
            case '4': color = (ColorRGB){255, 255, 255}; break; // White for another type of wall
            default: color = (ColorRGB){255, 255, 0}; break; // Yellow for default
        }
        if (side == 1)
        {
            color.r /= 2;
            color.g /= 2;
            color.b /= 2; // Darken color for side walls
        }
        for (int y = drawStart; y < drawEnd; y++)
        {
            // Assuming a function put_pixel exists to draw pixels on the screen
            mlx_pixel_put(data->mlx_ptr, data->win_ptr, x, y, rgb_to_hex(color.r, color.g, color.b));
        }
        x++;
    }
    oldTime = time;
    time = getTicks();
    double frameTime = (time - oldTime) / 1000.0; // Frame time in seconds
    data->rot_speed = frameTime * ROT_SPEED_MULTIPLIER; // Adjust rotation speed based on frame time
    data->move_speed = frameTime * MOVE_SPEED_MULTIPLIER; // Adjust movement speed based on frame time

    
}