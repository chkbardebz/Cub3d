/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/06 13:51:52 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double	dist(double ax, double ay, double bx, double by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

int	get_texture_pixel_fast(char *addr, int bpp, int sl, int x, int y)
{
	if (!addr || x < 0 || y < 0 || x >= 64 || y >= 64)
		return (0x000000);
	return (*(unsigned int *)(addr + (y * sl + x * (bpp / 8))));
}

/* Nouvelle fonction pour sélectionner la bonne adresse */
char	*get_texture_addr(t_data *data, void *texture)
{
	if (texture == data->no_img)
		return (data->no_addr);
	else if (texture == data->so_img)
		return (data->so_addr);
	else if (texture == data->we_img)
		return (data->we_addr);
	else if (texture == data->ea_img)
		return (data->ea_addr);
	return (data->no_addr);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	int		bytes;
	char	*dst;

	if (!data->addr)
	{
		fprintf(stderr, "ERROR: screen addr NULL\n");
		return ;
	}
	if (x < 0 || x >= data->w_width || y < 0 || y >= data->w_height)
		return ;
	bytes = data->bits_per_pixel / 8;
	dst = data->addr + (y * data->line_length + x * bytes);
	*(unsigned int *)dst = color;
}

int	check_map_bounds(t_data *data)
{
	int	px;
	int	py;

	if (!data->game_map_int || !data->game_map)
	{
		fprintf(stderr, "ERROR: game_map_int or game_map is NULL\n");
			//! ATTNETION
		return (0);
	}
	px = (int)(data->player_x / 64.0);
	py = (int)(data->player_y / 64.0);
	if (data->map_width <= 0 || data->map_height <= 0)
	{
		fprintf(stderr, "ERROR: Invalid map dimensions\n"); //! IDEM
		return (0);
	}
	if (px < 0 || py < 0 || px >= data->map_width || py >= data->map_height)
		return (0);
	return (1);
}

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

void	horizontal_ray_up(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{

	data->ray.dof = 0;
	data->ray.atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) - 0.0001;
	*hx = (data->player_y - *hy) * data->ray.atan + data->player_x;
	data->ray.yo = -64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*hx) >> 6;
		data->ray.my = (int)(*hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0 || data->ray.mx >= data->map_width || data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width * data->map_height
			&& data->game_map_int[data->ray.mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += data->ray.xo;
		*hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	horizontal_ray_down(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{
	data->ray.dof = 0;
	data->ray.atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) + 64;
	*hx = (data->player_y - *hy) * data->ray.atan + data->player_x;
	data->ray.yo = 64;
	data->ray.xo = -data->ray.yo * data->ray.atan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*hx) >> 6;
		data->ray.my = (int)(*hy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += data->ray.xo;
		*hy += data->ray.yo;
		data->ray.dof++;
	}
}

void	cast_horizontal_ray(t_data *data, double ra, double *disH, double *hx,
		double *hy)
{
	*disH = 10000;
	*hx = data->player_x;
	*hy = data->player_y;
	if (ra > PI)
		horizontal_ray_up(data, ra, disH, hx, hy);
	else if (ra < PI)
		horizontal_ray_down(data, ra, disH, hx, hy);
}

void	vertical_ray_left(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	data->ray.dof = 0;
	data->ray.ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) - 0.0001;
	*vy = (data->player_x - *vx) * data->ray.ntan + data->player_y;
	data->ray.xo = -64;
	data->ray.yo = -data->ray.xo * data->ray.ntan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*vx) >> 6;
		data->ray.my = (int)(*vy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += data->ray.xo;
		*vy += data->ray.yo;
		data->ray.dof++;
	}
}

void	vertical_ray_right(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	data->ray.dof = 0;
	data->ray.ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) + 64;
	*vy = (data->player_x - *vx) * data->ray.ntan + data->player_y;
	data->ray.xo = 64;
	data->ray.yo = -data->ray.xo * data->ray.ntan;
	while (data->ray.dof < 64)
	{
		data->ray.mx = (int)(*vx) >> 6;
		data->ray.my = (int)(*vy) >> 6;
		if (data->ray.mx < 0 || data->ray.my < 0
			|| data->ray.mx >= data->map_width
			|| data->ray.my >= data->map_height)
			break ;
		data->ray.mp = data->ray.my * data->map_width + data->ray.mx;
		if (data->ray.mp >= 0 && data->ray.mp < data->map_width
			* data->map_height && data->game_map_int[data->ray.mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += data->ray.xo;
		*vy += data->ray.yo;
		data->ray.dof++;
	}
}

void	cast_vertical_ray(t_data *data, double ra, double *disV, double *vx,
		double *vy)
{
	*disV = 10000;
	*vx = data->player_x;
	*vy = data->player_y;
	if (ra > P2 && ra < P3)
		vertical_ray_left(data, ra, disV, vx, vy);
	else if (ra < P2 || ra > P3)
		vertical_ray_right(data, ra, disV, vx, vy);
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

void	draw_column(t_data *data, int r, int line_h, int line_o, void *texture,
		int tex_x)
{
	int		draw_start;
	int		draw_end;
	int		y;
	int		tex_y;
	int		color;
	char	*tex_addr;

	tex_addr = get_texture_addr(data, texture);
	draw_start = (line_o < 0) ? 0 : line_o;
	draw_end = (line_o + line_h >= data->w_height) ? data->w_height - 1 : line_o
		+ line_h - 1;
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)(((y - line_o) * 64) / line_h);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= 64)
			tex_y = 63;
		color = get_texture_pixel_fast(tex_addr, data->tex_bpp, data->tex_sl,
				tex_x, tex_y);
		my_mlx_pixel_put(data, r, y, color);
		y++;
	}
}

void	calculate_wall_params(t_data *data, double ra)
{
	double	ca;

	ca = ra - data->p_orientation;
	data->ray.dis_t = data->ray.dis_t * cos(ca);
	data->ray.line_h = (int)((64 * data->w_height)
			/ (data->ray.dis_t > 1.0 ? data->ray.dis_t : 1.0));
	data->ray.line_o = (data->w_height / 2) - (data->ray.line_h / 2);
}

int	calculate_texture_x(double rx, double ry, int side, double ra)
{
	double	wall_x;
	int		tex_x;

	if (side == 0) // Mur vertical
	{
		wall_x = ry - floor(ry / 64.0) * 64.0;
		tex_x = (int)wall_x;
		// Inverser pour le mur Est (ra < P2 || ra > P3)
		if (ra > P2 && ra < P3)
			tex_x = 63 - tex_x;
	}
	else // Mur horizontal
	{
		wall_x = rx - floor(rx / 64.0) * 64.0;
		tex_x = (int)wall_x;
		// Inverser pour le mur Nord (ra < PI)
		if (ra < PI)
			tex_x = 63 - tex_x;
	}
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= 64)
		tex_x = 63;
	return (tex_x);
}

void	process_ray(t_data *data, int r, double ra)
{
	cast_horizontal_ray(data, ra, &data->ray.dis_h, &data->ray.hx,
		&data->ray.hy);
	cast_vertical_ray(data, ra, &data->ray.dis_v, &data->ray.vx, &data->ray.vy);
	if (data->ray.dis_v < data->ray.dis_h)
	{
		data->ray.rx = data->ray.vx;
		data->ray.ry = data->ray.vy;
		data->ray.dis_t = data->ray.dis_v;
		data->ray.side = 0;
	}
	else
	{
		data->ray.rx = data->ray.hx;
		data->ray.ry = data->ray.hy;
		data->ray.dis_t = data->ray.dis_h;
		data->ray.side = 1;
	}
	calculate_wall_params(data, ra);
	data->ray.tex_x = calculate_texture_x(data->ray.rx, data->ray.ry,
			data->ray.side, ra);
	data->ray.texture = select_texture(data, ra, data->ray.side);
	draw_column(data, r, data->ray.line_h, data->ray.line_o, data->ray.texture,
		data->ray.tex_x);
}

void	raycasting(t_data *data)
{
	double	fov;
	double	dr;
	double	ra;
	int		r;
	int		y;

	fov = 60 * (PI / 180);
	dr = fov / data->w_width;
	r = 0;
	while (r < data->w_width)
	{
		ra = data->p_orientation - (fov / 2) + r * dr;
		if (ra < 0)
			ra += 2 * PI;
		if (ra > 2 * PI)
			ra -= 2 * PI;
		process_ray(data, r, ra);
		r++;
	}
	y = 0;
	while (y < data->w_height) //! ligne rouge a la con a enlever
	{
		my_mlx_pixel_put(data, data->w_width / 2, y, 0xFF0000);
		y++;
	}
}
