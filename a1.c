#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int main()
{
    int fd_mensagem = atoi(argv[1]);
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

    Msg fim;
    fim.tipo = 2;
    fim.origem = 1;
    write(fd_mensagem, &fim, sizeof(Msg));

    return 0;
}