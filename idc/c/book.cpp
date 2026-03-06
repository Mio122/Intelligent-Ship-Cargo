#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig)
{
	printf("收到了%d信号\n", sig);
}

void fun(int n)
{
	printf("收到了%d时钟信号\n", n);
}

int main()
{
	for (int i = 0; i <= 64; i++)
	{
		signal(i, handler);
	}

	signal(SIGALRM, fun);
	alarm(3);

	while (1)
	{
		printf("执行了一次任务\n");

		sleep(1);
	}
	

}
