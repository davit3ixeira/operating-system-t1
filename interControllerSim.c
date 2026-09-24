#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "constants.h"

int main(int argc, char *argv[])
{
    int irq_fd = atoi(argv[1]);

    while(1){
        usleep(500000);

        Msg msg;

        msg.tipo = 0;
        msg.irq0 = 1;
        msg.irq1 = 0;
        msg.irq2 = 0;

        if((rand() % 100) < 10){
            msg.irq1 = 1;
        }
        
        if((rand() % 100) < 5){
            msg.irq2 = 1;
        }

        write(irq_fd, &msg, sizeof(Msg));
    }

    return 0;
}