/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:02:01 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:10:19 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	dist(double ax, double ay, double bx, double by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void	fill_background(t_data *img, int color1, int color2)
{
	int	x;
	int	y;

	y = 0;
	while (y < img->w_height / 2)
	{
		x = 0;
		while (x < img->w_width)
		{
			my_mlx_pixel_put(img, x, y, color1);
			x++;
		}
		y++;
	}
	while (y < img->w_height)
	{
		x = 0;
		while (x < img->w_width)
		{
			my_mlx_pixel_put(img, x, y, color2);
			x++;
		}
		y++;
	}
}

int	rgb_to_hex(int r, int g, int b)
{
	if (r < 0)
		r = 0;
	if (r > 255)
		r = 255;
	if (g < 0)
		g = 0;
	if (g > 255)
		g = 255;
	if (b < 0)
		b = 0;
	if (b > 255)
		b = 255;
	return ((r << 16) | (g << 8) | b);
}

void	print_tabtab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		printf("%s", tab[i]);
		i++;
	}
	printf("\n");
}

int	double_to_int(double value)
{
	return ((int)value);
}
