/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 04:02:01 by judenis           #+#    #+#             */
/*   Updated: 2025/08/08 16:48:13 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int rgb_to_hex(int r, int g, int b)
{
    // Assurer que les valeurs sont dans la plage 0-255
    if (r < 0) r = 0;
    if (r > 255) r = 255;
    if (g < 0) g = 0;
    if (g > 255) g = 255;
    if (b < 0) b = 0;
    if (b > 255) b = 255;
    
    // Convertir en hexadécimal : (R << 16) | (G << 8) | B
    return (r << 16) | (g << 8) | b;
}

void print_tabtab(char **tab)
{
    int i = 0;

    if (!tab)
        return;

    while (tab[i])
    {
        printf("%s", tab[i]);
        i++;
    }
    printf("\n");
}

int double_to_int(double value)
{
    return (int)value;
}
