#include "kernel/types.h"
#include "user/user.h"

int main(void)
{
        char *argv[] = {"echo", "this", "is", "echo", 0};
        exec(argv[0], argv);
        printf("exec failed!\n");
        exit(0);
}  