#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if(argc < 3){
    fprintf(2, "usage: nice <pid> <inc>\n");
    exit(1);
  }
  int ret = nice(atoi(argv[1]), atoi(argv[2]));
  printf("%d\n", ret);
  exit(0);
}
