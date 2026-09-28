#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "constants.h"

#define ID 5

void syscall_sim(int fd_msg, int fd_resp, int op, int *data, int PC){
    Msg m;
    int resposta;

    m.tipo = SYSCALL;
    m.origem = ID;
    m.op = op;
    m.valor = PC;

    write(fd_msg, &m, sizeof(Msg));
    read(fd_resp, &resposta, sizeof(int));

    if(op == R){
        *data = resposta;
    }
}

int main(int argc, char *argv[]){
    int fd_mensagem = atoi(argv[1]);
    int fd_resp = atoi(argv[2]);
    int *shm = shmat(atoi(argv[3]), NULL, 0);

    int PC = 0, N = 0, d, op;
    int *data;

    srand(getpid());

    while(PC < MAX){
        PC++;
        shm[2 * (ID - 1)] = PC;
        printf("A%d PC=%d\n", ID, PC);
        sleep(0.5);
        if ((d = rand()%100 + 1) < 15){
            if(d % 2){
                op = R;
                data = &N;
            }
            else{
                op = W;
                data = &PC;
            }
            syscall_sim(fd_mensagem, fd_resp, op, data, PC);
            if(op == R){
                shm[2 * (ID - 1) + 1] = N;
                printf("A%d recebeu N=%d\n", ID, N);
            }
        }
        sleep(0.5);
    }

    shmdt(shm);

    Msg fim;
    fim.tipo = FIM;
    fim.origem = ID;
    write(fd_mensagem, &fim, sizeof(Msg));

    return 0;
}