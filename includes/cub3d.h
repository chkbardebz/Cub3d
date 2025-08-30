/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 15:22:17 by ylouvel           #+#    #+#             */
/*   Updated: 2025/08/30 16:36:12 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define BUFFER_SIZE 42

# include "../libft/libft.h"
# include "mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>


#ifndef ROT_SPEED
# define ROT_SPEED 5
#endif

#ifndef POV_WIDTH
# define POV_WIDTH 120
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

#define ROT_SPEED_MULTIPLIER 3.0

#define MOVE_SPEED_MULTIPLIER 5.0

#define P2 PI/2

#define P3 3*PI/2

#define DR 0.0174533 // degree to radian conversion factor

#define TILE_SIZE 64
#define FOV_RAD   (60.0 * DR)  // 60° en radians

#define MOVE_SPEED 5


typedef struct s_data
{
	char	*filename;
	char **map;      // Fichier complet (textures + couleurs + carte)
	char **game_map; // Seulement la carte (lignes de 0, 1, espaces)
	int *game_map_int; // Version entière 1D de la carte
	void	*mlx_ptr;
	void	*win_ptr;
	int		w_width;
	int		w_height;
	int		map_width; // Largeur de la carte
	int		map_height; // Hauteur de la carte
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
    double  dir_x;
    double  dir_y;
	int		player_x;
	int		player_y;
	double	plane_x;
	double	plane_y;
	double 	pdx;
	double  pdy;
    double move_speed; // Speed of player movement
    double rot_speed;  // Speed of player rotation
	double p_orientation; // 0 = N, 90 = E, 180 = S, 270 = W

	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;

	
}			t_data;

// Function declarations
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

int rgb_to_hex(int r, int g, int b);

int double_to_int(double value);
void		game(void);
void raycasting(void);
int *convert_map_to_int(t_data *data);
void my_mlx_pixel_put(t_data *data, int x, int y, int color);

#endif
