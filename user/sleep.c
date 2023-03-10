#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int t;

  if(argc == 0)
    exit(-1);

  t = atoi(argv[1]);
  sleep(t);

  exit(0);
}
