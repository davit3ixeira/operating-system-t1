#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int main()
{
    int PC, N;
    PC = 1;

    while(PC < MAX){
        PC++;
        sleep(0.5);
        if (d = rand()%100 + 1 < 15){
            if(d % 2){
                Op = R;
                Data = &N;
            }
            else{
                Op = W;
                Data = &PC;
            }
        }
        sleep(0.5);
    }

    return 0;
}