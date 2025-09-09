/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:34:22 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:31:26 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (errormsg("Usage: ./cub3d <map_file>"));
	init(argv[1]);
	game();
	exit_game(0);
	return (0);
}
