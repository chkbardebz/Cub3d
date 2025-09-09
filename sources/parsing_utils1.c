/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/09 11:20:15 by judenis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

static int	find_start(char *line, int len)
{
	int	start;

	start = 0;
	while (start < len && is_whitespace(line[start]))
		start++;
	return (start);
}

static int	find_end(char *line, int start, int len)
{
	int	end;

	end = len - 1;
	while (end >= start && is_whitespace(line[end]))
		end--;
	return (end);
}

char	*safe_trim(char *line)
{
	int		start;
	int		end;
	int		len;
	char	*result;
	int		i;

	if (!line)
		return (NULL);
	len = ft_strlen(line);
	start = find_start(line, len);
	end = find_end(line, start, len);
	if (start > end)
		return (ft_strdup(""));
	result = malloc(end - start + 2);
	if (!result)
		return (NULL);
	i = -1;
	while (start <= end)
		result[++i] = line[start++];
	result[++i] = '\0';
	return (result);
}

int	is_map_line(char *line)
{
	int	i;

	if (!line || ft_strlen(line) == 0)
		return (0);
	i = 0;
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && line[i] != ' ' && line[i] != 'N'
			&& line[i] != 'S' && line[i] != 'E' && line[i] != 'W'
			&& line[i] != '\n' && line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}
