#include <stdio.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>


int main(void) {
    while (true) {
        int x = rand() % 10;

        if (x < 5) {
            printf("\033[92m%d", x);
        }
        else {
            printf("\033[0;30m%c", x);
        }        
    }
    return 0;
}