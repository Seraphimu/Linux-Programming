#include <stdio.h>

int main(void) {
    printf("Hello World!\n");
    //printf("Hello World!\n");
    //printf("Hello World!\n");
    //printf("Hello World!\n");
    //printf("Hello World!\n");
#ifdef SHOW
    printf("SHOW was defined\n");
# else
    printf("default\n");
#endif
    printf("end if\n");                                     


    return 0;
}
