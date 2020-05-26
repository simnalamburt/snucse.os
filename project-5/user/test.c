#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define O_RDONLY  0x000

int
main(void)
{
  if (fork() == 0) {
    // 자식
    char buf[16];
    read(open("README", O_RDONLY), buf, 15);
    buf[15] = 0;
    printf("received \"%s\"\n", buf);
  }

  sleep(10);
  exit(0);
}
