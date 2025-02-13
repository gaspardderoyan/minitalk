CC = cc
CLFAGS = -Wall -Wextra -Werror -Ilibft/inc

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: server client

client: client.c $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIBFT_DIR) -lft

server: server.c $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@ -L$(LIBFT_DIR) -lft
clean fclean:
	rm -f server client

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)


re: clean all
