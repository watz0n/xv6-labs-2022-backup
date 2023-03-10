#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dir, char* fn) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    switch(st.type){
        case T_FILE:
                fprintf(2, "find: first argument should a directory");
                exit(-1);
            break;

        case T_DIR:
            if(strlen(dir) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: dir-path too long\n");
                break;
            }
            strcpy(buf, dir);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(stat(buf, &st) < 0) {
                    printf("find: cannot stat %s\n", buf);
                    continue;
                }
                if(st.type==T_DIR && (p[0]=='.' && (p[1]=='.' || p[1]=='\0')))
                    continue;
                if(st.type==T_DIR) {
                    find(buf, fn);
                } else if(st.type==T_FILE) {
                    if(!strcmp(p, fn))
                        printf("%s\n", buf);
                }
            }
            break;
    }
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        fprintf(2, "find: find [base-dir] [file-name]");
        exit(-1);
    }
    find(argv[1], argv[2]);
    exit(0);
}