#include <signal.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
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
			
}

void	send_bit(void)
{
	uint8_t curr_bit;
	curr_bit = mt.c & (unsigned char)pow(2, 7 - mt.i);
	if (curr_bit)
		kill(mt.pid, SIGUSR2);
	else
		kill(mt.pid, SIGUSR1);
	mt.i++;
}

void	set_signal_action(void)
{
	struct sigaction	act;

	bzero(&act, sizeof(act));
	act.sa_handler = &signal_handler;

	sigaction(SIGUSR1, &act, NULL);
}

int	main(int ac, char **av)
{

	if (ac > 1)
		mt.pid = atoi(av[1]);

	char str[] = "test";
	mt.i = 0;
	mt.c = 0;
	mt.str = str;
	// mt.str = av[2];
	send_bit();
	while (1)
	{
		if (mt.i == 7)
		{
			mt.c++;
			mt.i = 0;
		}
		if (!mt.str[mt.c])
			return (0) ;
		continue ;
	}


	return (0);

}
