#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

int main()
{
    int fd_mensagem = atoi(argv[1]);
    int PC, N;
    PC = 0;

    while(PC < MAX){
        PC++;
    }

    Msg fim;
    fim.tipo = 2;
    fim.origem = 2;
    write(fd_mensagem, &fim, sizeof(Msg));

    return 0;
}