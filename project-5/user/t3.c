#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define PRINT_ADDR(x)   \
        printf("  "#x": va=%p pa=%p\n", (uint64)x, v2p((uint64)x));

int
main(int argc, char **argv)
{
  printf("before fork:\n");
  PRINT_ADDR(&main);
  sleep(10);

  if (fork() == 0) {
    if (fork() == 0) {
      printf("after fork (0)  :\n");
    } else {
      printf("after fork (1) :\n");
    }
  } else {
    if (fork() == 0) {
      printf("after fork (2)  :\n");
    } else {
      printf("after fork (3) :\n");
    }
  }
  PRINT_ADDR(&main);
  sleep(10);

  exit(0);
}
