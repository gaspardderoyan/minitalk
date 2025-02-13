// client.c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

struct metadata {
	int pid;
	int i;
	int c;
	char *str;
};

struct metadata mt;
void	send_bit(void);

void	signal_handler(int signal)
{
	if (signal == SIGUSR1)
		send_bit();
	if (signal == SIGUSR2)
	{
		write(1, "done!\n", 6);
		exit(0);
	}

}

void	send_bit(void)
{
	// TODO add something where if it's the last bit, we send it with a diff flag
	uint8_t curr_bit;
	curr_bit = (mt.str[mt.c] >> (7 - mt.i)) & 1; // Get the bit, shift it to the right
	if (curr_bit)
		kill(mt.pid, SIGUSR2);
	else
		kill(mt.pid, SIGUSR1);
	if (mt.i == 7)
	{
		mt.i = 0;
		mt.c++;
	}
	else
		mt.i++;
}

void	set_signal_action(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_handler;
	sigemptyset(&act.sa_mask);

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}


int	main(int ac, char **av)
{
	if (ac != 3) {
        fprintf(stderr, "Usage: %s <server_pid> <string>\n", av[0]); //error handling
        return 1;
    }

    mt.pid = atoi(av[1]); // Get server PID
    if (mt.pid <= 0) {
       fprintf(stderr, "Invalid PID\n");
        return 1;
    }

    mt.i = 0;
    mt.c = 0;
	mt.str = av[2];

    set_signal_action();
    send_bit();
    while (1)
	{
        pause();
    }

    return 0;
}
