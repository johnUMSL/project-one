#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void print_usage(const char *prog)
{
  printf("Usage: %s [-h] [-n proc] [-s simul] [-t iter]\n", prog);
}

static int launch_child(int iter)
{
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork");
    return -1;
  }
  if (pid == 0)
  {
    char iter_arg[32];
    snprintf(iter_arg, sizeof(iter_arg), "%d", iter);
    execl("./user", "user", iter_arg, (char *)NULL);
    perror("exec");
    _exit(1);
  }
  printf("OSS: launched PID %d\n", (int)pid);
  return 0;
}

int main(int argc, char *argv[])
{
  int opt;
  int proc = 1;
  int simul = 1;
  int iter = 1;

  while ((opt = getopt(argc, argv, "hn:s:t:")) != -1)
  {
    switch (opt)
    {
    case 'h':
      print_usage(argv[0]);
      return 0;
    case 'n':
      proc = atoi(optarg);
      break;
    case 's':
      simul = atoi(optarg);
      break;
    case 't':
      iter = atoi(optarg);
      break;
    default:
      print_usage(argv[0]);
      return 1;
    }
  }

  if (proc < 1)
  {
    proc = 1;
  }
  if (simul < 1)
  {
    simul = 1;
  }
  if (iter < 1)
  {
    iter = 1;
  }
  if (simul > proc)
  {
    simul = proc;
  }

  int running = 0;
  int launched = 0;
  int finished = 0;

  while (running < simul && launched < proc)
  {
    if (launch_child(iter) == 0)
    {
      running++;
      launched++;
    }
  }

  while (launched < proc)
  {
    wait(NULL);
    running--;
    finished++;
    if (launch_child(iter) == 0)
    {
      running++;
      launched++;
    }
  }

  while (running > 0)
  {
    wait(NULL);
    running--;
    finished++;
  }

  printf("OSS: summary launched %d finished %d\n", launched, finished);
  return 0;
}

// 0 < -s < 15
// 0 < -n < 100
// -t >= 0