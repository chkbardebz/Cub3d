/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:34:56 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 18:11:50 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	fill_black_screen(t_data *data)
{
	int	y;
	int	x;

	y = 0;
	while (y < data->w_height)
	{
		x = 0;
		while (x < data->w_width)
		{
			my_mlx_pixel_put(data, x, y, 0x000000);
			x++;
		}
		y++;
	}
}

void	*select_texture(t_data *data, double ra, int side)
{
	void	*texture;

	if (side == 0)
	{
		if (ra > P2 && ra < P3)
			texture = data->ea_img;
		else
			texture = data->we_img;
	}
	else
	{
		if (ra > PI)
			texture = data->so_img;
		else
			texture = data->no_img;
	}
	if (!texture)
		texture = data->no_img;
	return (texture);
}

int	draw_start_init(int line_o)
{
	if (line_o < 0)
		return (0);
	return (line_o);
}

int	draw_end_init(t_data *data, int line_h, int line_o)
{
	if (line_o + line_h >= data->w_height)
		return (data->w_height - 1);
	return (line_o + line_h - 1);
}

void	draw_column(t_data *data, int r, void *texture, int tex_x)
{
	int		y;
	int		tex_y;
	int		color;
	char	*tex_addr;

	tex_addr = get_texture_addr(data, texture);
	data->draw_start = draw_start_init(data->ray.line_o);
	data->draw_end = draw_end_init(data, data->ray.line_h, data->ray.line_o);
	y = data->draw_start;
	while (y <= data->draw_end)
	{
		tex_y = (int)(((y - data->ray.line_o) * 64) / data->ray.line_h);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= 64)
			tex_y = 63;
		color = get_texture_pixel_fast(tex_addr, data, tex_x, tex_y);
		my_mlx_pixel_put(data, r, y, color);
		y++;
	}
}
