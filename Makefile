CC = cc

CFLAGS =  -g3 -Wall -Wextra -Werror

INCLUDE = -I ./includes -I ./libft -I $(MLX_DIR)
DIRLIB = ./libft/
FILELIB = libft.a
NAMELFT = $(addprefix $(DIRLIB), $(FILELIB))

MLX_DIR = ./minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a

MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lbsd
MAKEFLAGS += --no-print-directory
RM = rm -f
NAME = cub3d

SRCDIR = sources/
CUB3D_SRC_FILES = main.c parsing.c init.c get_next_line.c exit.c error.c utils.c \
				game/loop.c game/raycasting.c

CUB3D_SRC = $(addprefix $(SRCDIR), $(CUB3D_SRC_FILES))
CUB3D_OBJ = $(CUB3D_SRC:.c=.o) 

all: $(NAME)

mlx :
				git clone https://github.com/42Paris/minilibx-linux.git minilibx
				@cd minilibx && ./configure

$(NAMELFT):
	@echo "Clonage et compilation de la libft..."
	@if [ ! -d "$(DIRLIB)" ]; then git clone https://github.com/YOUR_GITHUB/libft.git $(DIRLIB); fi
	@$(MAKE) -C $(DIRLIB)

$(MLX_DIR):
	@echo "Clonage de la MiniLibX..."
	@if [ ! -d "$(MLX_DIR)" ]; then git clone $(MLX_REPO) $(MLX_DIR); fi

$(MLX_LIB): $(MLX_DIR)
	@echo "Compilation de la MiniLibX..."
	@$(MAKE) -C $(MLX_DIR)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(NAME): $(CUB3D_OBJ) $(NAMELFT) $(MLX_LIB)
	@echo -e '\033[35mCreating Cub3d... 🕗\n'
	@$(CC) $(CFLAGS) $(CUB3D_OBJ) $(NAMELFT) $(MLX_FLAGS) -o $(NAME)
	@echo -e '\033[33;32mCub3d created ! 🎉\033[0m'

clean:
	@$(MAKE) -C $(DIRLIB) clean
	@$(RM) $(CUB3D_OBJ)
	@echo -e '\033[0;31mObject files deleted ! 🛑'

fclean: clean
	@$(MAKE) -C $(DIRLIB) fclean
	@$(RM) $(NAME)
	@echo -e '\033[0;31mCub3d deleted ! 🛑'

re: fclean all

.PHONY: all clean fclean re
