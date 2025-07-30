/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 03:34:54 by judenis           #+#    #+#             */
/*   Updated: 2025/07/30 03:35:23 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

int errormsg(const char *message)
{
    if (message)
    {
        write(2, "Error: ", 7);
        write(2, message, ft_strlen(message));
        write(2, "\n", 1);
    }
    return (1);
}