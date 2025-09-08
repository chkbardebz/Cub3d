/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 14:50:38 by judenis           #+#    #+#             */
/*   Updated: 2025/09/06 14:56:39 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

int	key_press_hook(int keycode, t_data *data)
{
	if (keycode == 119)
		data->key_w = 1;
	else if (keycode == 115)
		data->key_s = 1;
	else if (keycode == 97)
		data->key_a = 1;
	else if (keycode == 100)
		data->key_d = 1;
	else if (keycode == 65361)
		data->key_left = 1;
	else if (keycode == 65363)
		data->key_right = 1;
	else if (keycode == 65307)
		exit_game(0);
	return (0);
}

int	key_release_hook(int keycode, t_data *data)
{
	if (keycode == 119)
		data->key_w = 0;
	else if (keycode == 115)
		data->key_s = 0;
	else if (keycode == 97)
		data->key_a = 0;
	else if (keycode == 100)
		data->key_d = 0;
	else if (keycode == 65361)
		data->key_left = 0;
	else if (keycode == 65363)
		data->key_right = 0;
	return (0);
}

void	move_player(t_data *data, double *move_x, double *move_y,
		double *strafe_angle)
{
	if (data->key_w)
	{
		*move_x += cos(data->p_orientation) * MOVE_SPEED;
		*move_y += sin(data->p_orientation) * MOVE_SPEED;
	}
	if (data->key_s)
	{
		*move_x -= cos(data->p_orientation) * MOVE_SPEED;
		*move_y -= sin(data->p_orientation) * MOVE_SPEED;
	}
	if (data->key_a)
	{
		*strafe_angle = data->p_orientation - P2;
		*move_x += cos(*strafe_angle) * MOVE_SPEED;
		*move_y += sin(*strafe_angle) * MOVE_SPEED;
	}
	if (data->key_d)
	{
		*strafe_angle = data->p_orientation + P2;
		*move_x += cos(*strafe_angle) * MOVE_SPEED;
		*move_y += sin(*strafe_angle) * MOVE_SPEED;
	}
}

void	move_camera(t_data *data)
{
	if (data->key_left)
	{
		data->p_orientation -= 0.033;
		if (data->p_orientation < 0)
			data->p_orientation += 2 * PI;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
	}
	if (data->key_right)
	{
		data->p_orientation += 0.033;
		if (data->p_orientation > 2 * PI)
			data->p_orientation -= 2 * PI;
		data->pdx = cos(data->p_orientation) * 5;
		data->pdy = sin(data->p_orientation) * 5;
	}
}

void	update_movement(t_data *data)
{
	double	move_x;
	double	move_y;
	double	strafe_angle;
	double	new_x;
	double	new_y;

	move_x = 0;
	move_y = 0;
	move_player(data, &move_x, &move_y, &strafe_angle);
	new_x = data->player_x + move_x;
	new_y = data->player_y + move_y;
	collision(data, new_x, new_y);
	move_camera(data);
}
