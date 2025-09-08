/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: judenis <judenis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 10:00:00 by judenis           #+#    #+#             */
/*   Updated: 2025/09/08 19:47:19 by judenis          ###   ########.fr       */
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

int	is_valid_line(char *line)
{
	char	*trimmed;
	int		result;

	if (!line)
		return (0);
	trimmed = safe_trim(line);
	if (!trimmed)
		return (0);
	if (ft_strlen(trimmed) == 0)
	{
		free(trimmed);
		return (1);
	}
	result = (ft_strncmp(trimmed, "NO ", 3) == 0 || ft_strncmp(trimmed, "SO ",
				3) == 0 || ft_strncmp(trimmed, "WE ", 3) == 0
			|| ft_strncmp(trimmed, "EA ", 3) == 0 || ft_strncmp(trimmed, "F ",
				2) == 0 || ft_strncmp(trimmed, "C ", 2) == 0
			|| is_map_line(trimmed));
	free(trimmed);
	return (result);
}

int	check_parasites(char **map)
{
	int		i;
	char	*trimmed;

	i = 0;
	while (map[i])
	{
		if (!is_valid_line(map[i]))
		{
			trimmed = ft_strtrim(map[i], " \t\n\r");
			if (trimmed && ft_strlen(trimmed) > 0)
			{
				errormsg("Invalid element found in map file");
				free(trimmed);
				return (-1);
			}
			free(trimmed);
		}
		i++;
	}
	return (0);
}

int	height_map(char **game_map)
{
	int	height;

	if (!game_map)
		return (0);
	height = 0;
	while (game_map[height])
		height++;
	return (height);
}

int	width_map(char **game_map)
{
	int	max_width;
	int	current_width;
	int	i;

	if (!game_map)
		return (0);
	max_width = 0;
	i = 0;
	while (game_map[i])
	{
		current_width = ft_strlen(game_map[i]);
		if (current_width > 0 && (game_map[i][current_width - 1] == '\n'
			|| game_map[i][current_width - 1] == '\r'))
			current_width--;
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	return (max_width);
}
