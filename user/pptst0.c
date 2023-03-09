#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    int pp[2];
    int i;
    int n;
    int pd;

    pipe(pp);
    
    pd = fork();

    if(pd>0) {

        printf("D\n");
        close(pp[0]);

        printf("E\n");
        i = 1;
        n = write(pp[1], (void*)&i, 4);
        
        printf("F\n");
        close(pp[1]);

        pd = wait((int*)0);
        exit(0);

    } else if(pd==0) {
        
        printf("A\n");
        close(pp[1]);

        printf("B\n");
        n = read(pp[0], (void*)&i, 4); 
        printf("%d(%d)\n", i, n);
        n = read(pp[0], (void*)&i, 4); 
        printf("%d(%d)\n", i, n);

        printf("C\n");
        close(pp[0]);
        
        exit(0);

    } else {
        printf("fork error\n");        
    }
    exit(-2);
}