/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:34:22 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 05:30:03 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int main(int argc, char **argv)
{
    if (argc != 2)
        return errormsg("Usage: ./cub3d <map_file>");
    init(argv[1]);
    exit_game(0);

    // Initialize the game and start the main loop
    // ...

    return (0);
}