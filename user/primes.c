#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pp[2];
    int i;
    int p;
    int n;
    int pd;
    int pr;

    //int tc; //thread-count, debug only

    pipe(pp);
    //tc=0; 
    pd = fork(); 

    if(pd>0) {
        close(pp[0]);
        for(i=2; i<=35; i=i+1) {
            n = write(pp[1], (void*)&i, 4);
        }
        close(pp[1]);
        pd = wait((int*)0);
        exit(0);
    } else if(pd<0) {
        printf("fork error\n");  
    } else {
        while(pd==0) {
            close(pp[1]);
            pr = pp[0];
            p = 0;
            n = read(pr, (void*)&i, 4);
            if(n != 0) {
                p = i;
                printf("prime %d\n", p);
                pipe(pp);
                //tc=tc+1;
                pd = fork();
                if(pd) {
                    close(pp[0]);
                    while((n = read(pr, (void*)&i, 4))>0) {
                        if(i%p != 0) {
                            n = write(pp[1], (void*)&i, 4);
                        }
                    }
                    close(pp[1]);
                    close(pr);
                    pd = wait((int*)0);
                    exit(0);
                }
            } else {
                //printf("tc:%d\n", tc);
                close(pr);
                exit(0);
            }
        }
    } 

    exit(-2);
}
