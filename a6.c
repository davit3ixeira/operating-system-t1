#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "constants.h"

#define ID 6

int main(int argc, char *argv[])
{
    int fd_mensagem = atoi(argv[1]);
    int fd_resp     = atoi(argv[2]);

    int PC = 1, N = 0, d, op;
    int *data;

    srand(getpid());

    while(PC < MAX){
        printf("A%d PC=%d\n", ID, PC);
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
    fim.tipo = FIM;
    fim.origem = ID;
    write(fd_mensagem, &fim, sizeof(Msg));

    return 0;
}