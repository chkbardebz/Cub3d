/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:22:17 by ylouvel           #+#    #+#             */
/*   Updated: 2025/09/02 13:40:01 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define BUFFER_SIZE 42
# define ROT_SPEED 3
# define POV_WIDTH 120
# define PI 3.14159265358979323846
# define ROT_SPEED_MULTIPLIER 3.0
# define MOVE_SPEED_MULTIPLIER 5.0
# define P2 1.5707963267948966
# define P3 4.7123889803846899
# define DR 0.0174533
# define TILE_SIZE 64
# define FOV_RAD 1.0471975511965979
# define MOVE_SPEED 2

# include "../libft/libft.h"
# include "mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_data
{
	char	*filename;
	char	**map;
	char	**game_map;
	int		*game_map_int;
	void	*mlx_ptr;
	void	*win_ptr;
	int		w_width;
	int		w_height;
	int		map_width;
	int		map_height;
	char	*no_texture;
	char	*so_texture;
	char	*we_texture;
	char	*ea_texture;
	void	*no_img;
	void	*so_img;
	void	*we_img;
	void	*ea_img;
	int		f_color[3];
	int		c_color[3];
	double	dir_x;
	double	dir_y;
	double	player_x;
	double	player_y;
	double	plane_x;
	double	plane_y;
	double	pdx;
	double	pdy;
	double	move_speed;
	double	rot_speed;
	double	p_orientation;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		key_w;
	int		key_s;
	int		key_a;
	int		key_d;
	int		key_left;
	int		key_right;
}			t_data;

int			errormsg(const char *message);
void		init(const char *map_file);
int			exit_game(int return_value);
t_data		*get_data(void);
char		*get_next_line(int fd);
void		print_tabtab(char **tab);
void		free_tabtab(char **tab);
int			iscorrectformat(const char *filename);
int			parsing(t_data *data);
char		**extract_map_only(char **full_map);
int			height_map(char **game_map);
int			width_map(char **game_map);
int			verif_map(t_data *data);
int			rgb_to_hex(int r, int g, int b);
int			double_to_int(double value);
void		game(void);
void		raycasting(void);
int			*convert_map_to_int(t_data *data);
void		my_mlx_pixel_put(t_data *data, int x, int y, int color);
int			key_press_hook(int keycode, t_data *data);
int			key_release_hook(int keycode, t_data *data);
int			game_loop(t_data *data);
void		update_movement(t_data *data);
char		*safe_trim(char *line);
int			is_map_line(char *line);
int			is_valid_line(char *line);
int			check_parasites(char **map);
int			check_map_position(char **map);
int			check_elements_before_map(char **map);
int			get_textures(t_data *data, char **map);
int			get_color(t_data *data);
int			init_orientation(t_data *data);
int			check_map_closed(t_data *data);

#endif
