CC = cc
CFLAGS = -g3 -Wall -Wextra -Werror

INCLUDE = -I ./includes -I ./libft -I $(MLX_DIR)

DIRLIB = ./libft
FILELIB = libft.a
NAMELFT = $(DIRLIB)/$(FILELIB)

MLX_DIR = ./minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd

MAKEFLAGS += --no-print-directory
RM = rm -f
NAME = cub3D

SRCDIR = sources/
CUB3D_SRC_FILES = main.c parsing.c init.c get_next_line.c exit.c error.c utils.c \
				game/loop.c game/raycasting.c map_conversion.c \
				parsing_checks.c parsing_utils.c parsing_player.c parsing_colors.c parsing_textures.c \
				game/collision.c game/movement.c game/horizontal_ray.c game/vertical_ray.c game/draw.c

CUB3D_SRC = $(addprefix $(SRCDIR), $(CUB3D_SRC_FILES))
CUB3D_OBJ = $(CUB3D_SRC:.c=.o)

all: $(NAME)

$(NAME): $(CUB3D_OBJ) $(NAMELFT) $(MLX_LIB)
	@echo -e '\033[35mCreating Cub3d... 🕗\n'
	@$(CC) $(CFLAGS) $(CUB3D_OBJ) $(NAMELFT) $(MLX_FLAGS) -o $(NAME)
	@echo -e '\033[33;32mCub3d created ! 🎉\033[0m'

$(NAMELFT):
	@if [ ! -d "$(DIRLIB)" ]; then \
		echo "Clonage de libft..."; \
		git clone https://github.com/YOUR_GITHUB/libft.git $(DIRLIB); \
	fi
	@$(MAKE) -C $(DIRLIB)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "Clonage de la MiniLibX..."; \
		git clone https://github.com/42Paris/minilibx-linux.git $(MLX_DIR); \
	fi
	@$(MAKE) -C $(MLX_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@$(MAKE) -C $(DIRLIB) clean
	@$(RM) $(CUB3D_OBJ)
	@echo -e '\033[0;31mObject files deleted ! 🛑'

fclean: clean
	@$(MAKE) -C $(DIRLIB) fclean
	@$(MAKE) -C $(MLX_DIR) clean
	@$(RM) $(NAME)
	@echo -e '\033[0;31mCub3d deleted ! 🛑'

re: fclean all

.PHONY: all clean fclean re