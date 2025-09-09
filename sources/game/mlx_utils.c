/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 11:15:32 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:16:20 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	int		bytes;
	char	*dst;

	if (!data->addr)
		exit_game(errormsg("Screen addr NULL"));
	if (x < 0 || x >= data->w_width || y < 0 || y >= data->w_height)
		return ;
	bytes = data->bits_per_pixel / 8;
	dst = data->addr + (y * data->line_length + x * bytes);
	*(unsigned int *)dst = color;
}

int	get_texture_pixel_fast(char *addr, t_data *data, int x, int y)
{
	if (!addr || x < 0 || y < 0 || x >= 64 || y >= 64)
		return (0x000000);
	return (*(unsigned int *)(addr + (y * data->tex_sl + x * (data->tex_bpp
				/ 8))));
}

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
