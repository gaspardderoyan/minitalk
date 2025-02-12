#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <math.h>

struct current_byte {
	u_int8_t bytes;
	u_int8_t counter;
};

struct current_byte curr_b;

void	signal_handler(int signal)
{
	if (signal == SIGUSR1)
	{
		curr_b.counter++;
		printf("SIGUSR1 - 0\n");
		printf("counter: %d\n", curr_b.counter);
		printf("byte: %d\n", curr_b.bytes);
	}
	if (signal == SIGUSR2)
	{
		curr_b.bytes = curr_b.bytes | (u_int8_t)pow(2, 7 - curr_b.counter);
		curr_b.counter++;
		printf("SIGUSR2 - 1\n");
		printf("counter: %d\n", curr_b.counter);
		printf("byte: %d\n", curr_b.bytes);
	}
}

void	set_signal_action(void)
{
	struct sigaction	act;

	bzero(&act, sizeof(act));
	act.sa_handler = &signal_handler;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

int	main(void)
{
	pid_t pid = getpid();
	printf("PID: %i\n", pid);

	curr_b.bytes = 0;
	curr_b.counter = 0;

	set_signal_action();
	while (1)
	{
		if (curr_b.counter == 7)
		{
			printf("char: -%c-\n", (char)curr_b.bytes);
			curr_b.bytes = 0;
			curr_b.counter = 0;
		}
		continue ;
	}
	return (0);
}
