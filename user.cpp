// Author: John Garrett
// Date: 2026-02-10
// Description: Child process that reports its PID/PPID across iterations.

#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
  // Default to a single iteration if no argument provided.
  int iterations = 1;

  // Parse optional iteration count from argv.
  if (argc > 1)
  {
    iterations = atoi(argv[1]);
  }
  if (iterations < 1)
  {
    iterations = 1;
  }

  // Capture process identifiers for logging.
  pid_t pid = getpid();
  pid_t ppid = getppid();

  // Loop for the requested number of iterations.
  for (int i = 1; i <= iterations; i++)
  {
    // Print before and after a one-second sleep.
    cout << "USER PID:" << static_cast<int>(pid)
         << " PPID:" << static_cast<int>(ppid)
         << " Iteration:" << i << " before sleeping" << endl;
    sleep(1);
    cout << "USER PID:" << static_cast<int>(pid)
         << " PPID:" << static_cast<int>(ppid)
         << " Iteration:" << i << " after sleeping" << endl;
  }

  return 0;
}
