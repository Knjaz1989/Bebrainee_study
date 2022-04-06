#include <stdio.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>


int main(void)
{
        while (true)
    {
        int x = rand() % 30;

        if (x < 9)
        {
            printf("\033[92m %d", x);
        }
        else
        {
            printf("\033[92m %c", ' ');
        }        
    }
    
    return 0;
}