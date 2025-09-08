/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_colors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 19:01:45 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	is_in_range(int r, int g, int b, int color[3])
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0);
	color[0] = r;
	color[1] = g;
	color[2] = b;
	return (1);
}

static int	parse_color(char *line, int color[3], char *err)
{
	char	**split;
	int		r;
	int		g;
	int		b;

	split = ft_split(line, ',');
	if (!split || !split[0] || !split[1] || !split[2] || split[3]
		|| ft_strncmp(split[2], "\n", 1) == 0)
	{
		if (split)
			free_tabtab(split);
		errormsg(err);
		return (-1);
	}
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	free_tabtab(split);
	if (!is_in_range(r, g, b, color))
	{
		errormsg(err);
		return (-1);
	}
	return (0);
}

int	get_color(t_data *data)
{
	int	i;

	i = 0;
	while (data->map[i])
	{
		if (ft_strncmp(data->map[i], "F ", 2) == 0 && parse_color(data->map[i]
				+ 2, data->f_color, "Invalid floor color") == -1)
			return (-1);
		if (ft_strncmp(data->map[i], "C ", 2) == 0 && parse_color(data->map[i]
				+ 2, data->c_color, "Invalid ceiling color") == -1)
			return (-1);
		i++;
	}
	return (0);
}
