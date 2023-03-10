#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int cpd;
  int fpd;
  int pp[2];
  char bf;
  int bl;

  pipe(pp);
  fpd = fork();
  cpd = getpid();
  
  bf = 0;
  bl = 0;

  if(fpd>0) {
    bl = read(pp[0], &bf, 1);
    fpd = wait((int*)0); //wait child finish
    if(bl==1 && bf==0xa5) {
        printf("%d: received pong\n", cpd);
        exit(0);
    } else {
        exit(-1);
    }
  } else if(fpd==0) {
    bf = 0xa5;
    bl = write(pp[1], &bf, 1);
    if(bl==1) {
        printf("%d: received ping\n", cpd);
        exit(0);
    } else {
        exit(-1);
    }
  } else {
    printf("fork error\n");
  }

  exit(-2);
}
