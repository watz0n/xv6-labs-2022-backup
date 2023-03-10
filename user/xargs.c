#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char* argv[]) {
    struct stat st;
    char c;
    char *s;
    char *p;
    uint l;
    int i;
    int pd;
    char* xargv[MAXARG+1];

    if(!(fstat(0, &st) < 0)) {
        fprintf(2, "xargs: expect input from pipe\n");
        exit(-1);
    }

    l = 64;
    s = malloc(l);
    memset(s, 0, l);
    p = s;

    while(read(0, &c, 1)>0) {
        if(c=='\n') {
            memset(xargv, 0, sizeof(char*)*MAXARG);
            for(i=0; (i+1)<argc; i=i+1)
                xargv[i] = argv[i+1];
            xargv[i] = s;
            pd = fork();
            if(pd>0) {
                pd = wait((int*)0);
                memset(s, 0, l);
                p = s;
            } 
            else if(pd==0) {
                exec(xargv[0], xargv);
                printf("exec error\n");
                exit(-1);
            } 
            else {
                fprintf(2, "xargs: fork error");
                exit(-1);
            }
        } 
        else {
            *p++ = c;
            if(p==(s+l)) { //extend 64 byte
                p = malloc(l+64);
                memcpy(p, s, l);
                p = s+l;
                l = l+64;
            }
        }           
    }

    if(s) free(s);
    exit(0);
}