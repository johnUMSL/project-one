// Author: John Garrett
// Date: 2026-02-10
// Description: Launches and coordinates child processes with configurable limits.

#include <getopt.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

static void print_usage(const char *prog)
{
  // Print valid CLI usage for this program.
  cout << "Usage: " << prog << " [-h] [-n proc] [-s simul] [-t iter]" << endl;
}

// Spawn a single child process running the user program.
static int launch_child(int iter)
{
  // Fork the process; the child will exec into the user program.
  pid_t pid = fork();
  if (pid < 0)
  {
    perror("fork");
    return -1;
  }
  if (pid == 0)
  {
    // Convert iteration count to a string for exec argument.
    char iter_arg[32];
    snprintf(iter_arg, sizeof(iter_arg), "%d", iter);

    // Replace child image with ./user; only returns on error.
    execlp("./user", "user", iter_arg, static_cast<char *>(nullptr));
    perror("exec");
    exit(1);
  }
  // Parent continues here after successful fork.
  cout << "OSS: launched PID " << static_cast<int>(pid) << endl;
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
      // Help flag: print usage and exit.
      print_usage(argv[0]);
      return 0;
    case 'n':
      // Total number of processes to launch.
      proc = atoi(optarg);
      break;
    case 's':
      // Maximum number of concurrent processes.
      simul = atoi(optarg);
      break;
    case 't':
      // Iterations passed to each user process.
      iter = atoi(optarg);
      break;
    default:
      // Unknown flag: show usage and exit with error.
      print_usage(argv[0]);
      return 1;
    }
  }

  if (proc < 1)
  {
    // Enforce minimum process count.
    proc = 1;
  }
  if (simul < 1)
  {
    // Enforce minimum concurrency.
    simul = 1;
  }
  if (iter < 0)
  {
    // Disallow negative iterations.
    iter = 1;
  }
  if (simul > proc)
  {
    // Concurrency cannot exceed total processes.
    simul = proc;
  }

  int running = 0;
  int launched = 0;
  int finished = 0;

  // Start initial batch up to the concurrent limit.
  while (running < simul && launched < proc)
  {
    if (launch_child(iter) == 0)
    {
      running++;
      launched++;
    }
  }

  // Maintain the concurrency window until all processes are launched.
  while (launched < proc)
  {
    // Block until any child exits.
    wait(nullptr);
    running--;
    finished++;
    if (launch_child(iter) == 0)
    {
      running++;
      launched++;
    }
  }

  // Wait for remaining children to finish.
  while (running > 0)
  {
    // Drain remaining children.
    wait(nullptr);
    running--;
    finished++;
  }

  cout << "OSS: summary launched " << launched << " finished " << finished << endl;
  return 0;
}
