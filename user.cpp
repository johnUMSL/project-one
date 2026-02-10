#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int iterations = 1;

  if (argc > 1)
  {
    iterations = atoi(argv[1]);
    if (iterations < 1)
    {
      iterations = 1;
    }
  }

  pid_t pid = getpid();
  pid_t ppid = getppid();

  for (int i = 1; i <= iterations; i++)
  {
    printf("USER PID:%d PPID:%d Iteration:%d before sleeping\n",
           (int)pid, (int)ppid, i);
    sleep(1);
    printf("USER PID:%d PPID:%d Iteration:%d after sleeping\n",
           (int)pid, (int)ppid, i);
  }

  return 0;
}
