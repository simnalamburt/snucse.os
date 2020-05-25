#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PRINT_ADDR(x)   \
        fprintf(2, "  "#x": va=%p pa=%p\n", (uint64)x, v2p((uint64)x));

int
main(int argc, char **argv)
{
  fprintf(2, "before fork:\n");
  PRINT_ADDR(&main);
  sleep(10);

  int pid = fork();
  if (pid == 0) {
    fprintf(2, "after fork (child)  :\n");
  } else {
    fprintf(2, "after fork (parent) :\n");
  }
  PRINT_ADDR(&main);
  sleep(10);

  if (pid == 0) {
    exec("t2", (char*[]) { });
  }
  sleep(20);

  exit(0);
}
