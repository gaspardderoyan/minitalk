#include "libft/inc/libft.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>

struct current_byte {
	u_int8_t				bytes;
	u_int8_t				counter;
	volatile sig_atomic_t	received_signal;
	int						current_pid;
};

struct metadata {
	int pid;
	int i;
	int c;
	char *str;
};
