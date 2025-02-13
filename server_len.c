// server.c
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <stdio.h>
#include <signal.h>

struct current_byte {
	u_int8_t bytes;
	u_int8_t counter;
	volatile sig_atomic_t received_signal;
	unsigned long long len;
	unsigned long long curr_len;
};

struct current_byte curr_b;
void	set_len_signal_action(void);

void	len_signal_handler(int signum, siginfo_t *info, void *context)
{
	if (signum == SIGUSR1)
	{
		curr_b.counter++;
	}
	if (signum == SIGUSR2)
	{
		curr_b.bytes |= (1 << (sizeof(unsigned long long) - curr_b.counter));
		curr_b.counter++;
	}

	curr_b.received_signal = 1;
	if (curr_b.counter == sizeof(unsigned long long) + 1)
	{
		curr_b.counter = 1;
		set_len_signal_action();
	}
	kill(info->si_pid, SIGUSR1);
}

void	signal_handler(int signum, siginfo_t *info, void *context)
{
	if (signum == SIGUSR1)
	{
		curr_b.counter++;
	}
	if (signum == SIGUSR2)
	{
		curr_b.bytes |= (1 << (8 - curr_b.counter));
		curr_b.counter++;
	}

	curr_b.received_signal = 1;
	if (curr_b.counter == 9)
	{
		write(1, &curr_b.bytes, 1);
		curr_b.curr_len++;
		curr_b.bytes = 0;
		curr_b.counter = 1;
	}
	if (curr_b.curr_len == curr_b.len)
	{
		write(1, "\n", 1);
		kill(info->si_pid, SIGUSR2);
		return ;
	}
	kill(info->si_pid, SIGUSR1);
}

void	set_len_signal_action(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &len_signal_handler;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

void	set_signal_action(void)
{
	struct sigaction	act;

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = &signal_handler;
	act.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

int	main(void)
{
	pid_t pid = getpid();
	printf("PID: %i\n", pid);

	curr_b.bytes = 0;
	curr_b.counter = 1;
	curr_b.received_signal = 0;
	curr_b.len = 0;
	curr_b.curr_len = 0;

	set_len_signal_action();
	while (1)
	{
		while (!curr_b.received_signal)
			pause();
		curr_b.received_signal = 0;
	}
	return (0);
}
