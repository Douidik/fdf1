NAME = fdf

LIBFT_DIR = ./libft
FT_PRINTF_DIR = ./ft_printf
MLX_DIR = ./minilibx-linux
LIBFT = $(LIBFT_DIR)/libft.a
FT_PRINTF = $(FT_PRINTF_DIR)/libftprintf.a
MLX = $(MLX_DIR)/libmlx.a

FILES_C = \
fdf.c \
main.c \
map.c \
mat.c \
mat_mul.c \
mat_projection.c \
parse.c \
render.c \
scan.c \
util.c \
vec.c \
window.c \
camera.c \
render_map.c \
render_ui.c \
render_debug.c

SRC_DIR = ./src
BIN_DIR = ./bin

CC = cc
SRC = $(addprefix $(SRC_DIR)/,$(FILES_C))
OBJ = $(addprefix $(BIN_DIR)/,$(FILES_C:.c=.o))
INC = -I $(SRC_DIR) -I $(LIBFT_DIR) -I $(FT_PRINTF_DIR) -I $(MLX_DIR)

#CFLAGS = -g -Wall -Wextra -Werror -lXext -lX11 -lm -lz
CFLAGS = -g -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(LIBFT) $(FT_PRINTF) $(MLX) $(BIN_DIR) $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(INC) $(LIBFT) $(FT_PRINTF) $(MLX)

$(LIBFT):
	make -C $(LIBFT_DIR) bonus

$(FT_PRINTF):
	make -C $(FT_PRINTF_DIR) bonus

$(MLX):
	make -C $(MLX_DIR)

$(BIN_DIR):
	mkdir -p $(dir $(OBJ))

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INC)

clean:
	make -C $(LIBFT_DIR) clean
	make -C $(FT_PRINTF_DIR) clean
	rm -rf $(BIN_DIR)/*.o

fclean: clean
	make -C $(LIBFT_DIR) fclean
	make -C $(FT_PRINTF_DIR) fclean
	rm -rf $(NAME)

re: fclean all

.PHONY = all clean fclean re
