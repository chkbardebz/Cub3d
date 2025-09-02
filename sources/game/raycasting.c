/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 16:21:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/02 13:29:00 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

double	dist(double ax, double ay, double bx, double by)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

int	get_texture_pixel(void *img, int x, int y)
{
	int		bpp;
	int		sl;
	int		endian;
	char	*addr;

	if (!img || x < 0 || y < 0 || x >= 64 || y >= 64)
		return (0x000000);
	addr = mlx_get_data_addr(img, &bpp, &sl, &endian);
	if (!addr)
		return (0x000000);
	return (*(unsigned int *)(addr + (y * sl + x * (bpp / 8))));
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

static int	check_map_bounds(t_data *data)
{
	int	px;
	int	py;
	int	map_w;
	int	map_h;

	if (!data->game_map_int || !data->game_map)
	{
		fprintf(stderr, "ERROR: game_map_int or game_map is NULL\n");
		return (0);
	}
	px = (int)(data->player_x / 64.0);
	py = (int)(data->player_y / 64.0);
	map_w = data->map_width;
	map_h = data->map_height;
	if (map_w <= 0 || map_h <= 0)
	{
		fprintf(stderr, "ERROR: Invalid map dimensions\n");
		return (0);
	}
	if (px < 0 || py < 0 || px >= map_w || py >= map_h)
		return (0);
	return (1);
}

static void	fill_black_screen(t_data *data)
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

static void	horizontal_ray_up(t_data *data, double ra, double *disH,
	double *hx, double *hy)
{
	int		dof;
	double	atan;
	double	yo;
	double	xo;
	int		mx;
	int		my;
	int		mp;

	dof = 0;
	atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) - 0.0001;
	*hx = (data->player_y - *hy) * atan + data->player_x;
	yo = -64;
	xo = -yo * atan;
	while (dof < 8)
	{
		mx = (int)(*hx) >> 6;
		my = (int)(*hy) >> 6;
		if (mx < 0 || my < 0 || mx >= data->map_width || my >= data->map_height)
			break ;
		mp = my * data->map_width + mx;
		if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += xo;
		*hy += yo;
		dof++;
	}
}

static void	horizontal_ray_down(t_data *data, double ra, double *disH,
	double *hx, double *hy)
{
	int		dof;
	double	atan;
	double	yo;
	double	xo;
	int		mx;
	int		my;
	int		mp;

	dof = 0;
	atan = -1 / tan(ra);
	*hy = (((int)data->player_y >> 6) << 6) + 64;
	*hx = (data->player_y - *hy) * atan + data->player_x;
	yo = 64;
	xo = -yo * atan;
	while (dof < 8)
	{
		mx = (int)(*hx) >> 6;
		my = (int)(*hy) >> 6;
		if (mx < 0 || my < 0 || mx >= data->map_width || my >= data->map_height)
			break ;
		mp = my * data->map_width + mx;
		if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
		{
			*disH = dist(data->player_x, data->player_y, *hx, *hy);
			break ;
		}
		*hx += xo;
		*hy += yo;
		dof++;
	}
}

static void	cast_horizontal_ray(t_data *data, double ra, double *disH,
	double *hx, double *hy)
{
	*disH = 1000000;
	*hx = data->player_x;
	*hy = data->player_y;
	if (ra > PI)
		horizontal_ray_up(data, ra, disH, hx, hy);
	else if (ra < PI)
		horizontal_ray_down(data, ra, disH, hx, hy);
}

static void	vertical_ray_left(t_data *data, double ra, double *disV,
	double *vx, double *vy)
{
	int		dof;
	double	ntan;
	double	xo;
	double	yo;
	int		mx;
	int		my;
	int		mp;

	dof = 0;
	ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) - 0.0001;
	*vy = (data->player_x - *vx) * ntan + data->player_y;
	xo = -64;
	yo = -xo * ntan;
	while (dof < 8)
	{
		mx = (int)(*vx) >> 6;
		my = (int)(*vy) >> 6;
		if (mx < 0 || my < 0 || mx >= data->map_width || my >= data->map_height)
			break ;
		mp = my * data->map_width + mx;
		if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += xo;
		*vy += yo;
		dof++;
	}
}

static void	vertical_ray_right(t_data *data, double ra, double *disV,
	double *vx, double *vy)
{
	int		dof;
	double	ntan;
	double	xo;
	double	yo;
	int		mx;
	int		my;
	int		mp;

	dof = 0;
	ntan = -tan(ra);
	*vx = (((int)data->player_x >> 6) << 6) + 64;
	*vy = (data->player_x - *vx) * ntan + data->player_y;
	xo = 64;
	yo = -xo * ntan;
	while (dof < 8)
	{
		mx = (int)(*vx) >> 6;
		my = (int)(*vy) >> 6;
		if (mx < 0 || my < 0 || mx >= data->map_width || my >= data->map_height)
			break ;
		mp = my * data->map_width + mx;
		if (mp >= 0 && mp < data->map_width * data->map_height && data->game_map_int[mp] == 1)
		{
			*disV = dist(data->player_x, data->player_y, *vx, *vy);
			break ;
		}
		*vx += xo;
		*vy += yo;
		dof++;
	}
}

static void	cast_vertical_ray(t_data *data, double ra, double *disV,
	double *vx, double *vy)
{
	*disV = 1000000;
	*vx = data->player_x;
	*vy = data->player_y;
	if (ra > P2 && ra < P3)
		vertical_ray_left(data, ra, disV, vx, vy);
	else if (ra < P2 || ra > P3)
		vertical_ray_right(data, ra, disV, vx, vy);
}

static void	*select_texture(t_data *data, double ra, int side)
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

static void	draw_column(t_data *data, int r, int line_h, int line_o,
	void *texture, int tex_x)
{
	int	draw_start;
	int	draw_end;
	int	y;
	int	tex_y;
	int	color;

	draw_start = (line_o < 0) ? 0 : line_o;
	draw_end = (line_o + line_h >= data->w_height) ? 
		data->w_height - 1 : line_o + line_h - 1;
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)(((y - line_o) * 64) / line_h);
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= 64)
			tex_y = 63;
		color = get_texture_pixel(texture, tex_x, tex_y);
		my_mlx_pixel_put(data, r, y, color);
		y++;
	}
}

static void	calculate_wall_params(t_data *data, double dis_t, double ra,
	int *line_h, int *line_o)
{
	double	ca;

	ca = ra - data->p_orientation;
	dis_t = dis_t * cos(ca);
	*line_h = (int)((64 * data->w_height) / (dis_t > 0.0001 ? dis_t : 0.0001));
	*line_o = (data->w_height / 2) - (*line_h / 2);
}

static int	calculate_texture_x(double rx, double ry, int side)
{
	double	wall_x;
	int		tex_x;

	if (side == 0)
		wall_x = ry - floor(ry / 64.0) * 64.0;
	else
		wall_x = rx - floor(rx / 64.0) * 64.0;
	tex_x = (int)wall_x;
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= 64)
		tex_x = 63;
	return (tex_x);
}

static void	process_ray(t_data *data, int r, double ra)
{
	double	dis_h;
	double	dis_v;
	double	hx;
	double	hy;
	double	vx;
	double	vy;
	double	rx;
	double	ry;
	double	dis_t;
	int		side;
	int		line_h;
	int		line_o;
	int		tex_x;
	void	*texture;

	cast_horizontal_ray(data, ra, &dis_h, &hx, &hy);
	cast_vertical_ray(data, ra, &dis_v, &vx, &vy);
	if (dis_v < dis_h)
	{
		rx = vx;
		ry = vy;
		dis_t = dis_v;
		side = 0;
	}
	else
	{
		rx = hx;
		ry = hy;
		dis_t = dis_h;
		side = 1;
	}
	calculate_wall_params(data, dis_t, ra, &line_h, &line_o);
	tex_x = calculate_texture_x(rx, ry, side);
	texture = select_texture(data, ra, side);
	draw_column(data, r, line_h, line_o, texture, tex_x);
}

void	raycasting(void)
{
	t_data	*data;
	double	fov;
	double	dr;
	double	ra;
	int		r;
	int		y;

	data = get_data();
	if (!check_map_bounds(data))
	{
		fill_black_screen(data);
		return ;
	}
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
	while (y < data->w_height)
	{
		my_mlx_pixel_put(data, data->w_width / 2, y, 0xFF0000);
		y++;
	}
}
