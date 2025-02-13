CC = cc
CLFAGS = -Wall -Wextra -Werror

all: server client

client: client.c
	$(CC) $(CFLAGS) $^ -o $@

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

clean fclean:
	rm -f server client

re: clean all
