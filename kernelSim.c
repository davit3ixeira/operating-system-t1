#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "constants.h"

pid_t pid1, pid2, pid3, pid4, pid5, pid6, pid_irq;
int pid = 1;
int cont_recv = 0, fila_recv[6];
int cont_send = 0, fila_send[6];
int status[6];
int finalizados = 0;

// parceiro[6] = {1,0,3,2,5,4} -- mapa fixo A1<->A2, A3<->A4, A5<->A6
// buf_A1_A2 etc viram um array buf[6] indexado por processo
// pc_pendente[6] -- guarda o PC de cada processo ao pedir send(), usado no IRQ2

void pausar(int pidN){
    if(pidN == 1){
        kill(pid1, SIGSTOP);
    }
    else if(pidN == 2){ 
        kill(pid2, SIGSTOP);
    }
    else if(pidN == 3){
        kill(pid3, SIGSTOP);
    }
    else if(pidN == 4){
        kill(pid4, SIGSTOP);
    }
    else if(pidN == 5){
        kill(pid5, SIGSTOP);
    }
    else if(pidN == 6){
        kill(pid6, SIGSTOP);
    }
}

void continuar(int pidN){
    if(pidN == 1){
        kill(pid1, SIGCONT);
    }
    else if(pidN == 2){ 
        kill(pid2, SIGCONT);
    }
    else if(pidN == 3){
        kill(pid3, SIGCONT);
    }
    else if(pidN == 4){
        kill(pid4, SIGCONT);
    }
    else if(pidN == 5){
        kill(pid5, SIGCONT);
    }
    else if(pidN == 6){
        kill(pid6, SIGCONT);
    }
}

void enfileirar(int pidN, int fila[], int *cont){
    fila[*cont] = pidN;
    (*cont)++;
}

int desenfileirar(int fila[], int *cont){
    int pidN = fila[0];

    fila[0] = fila[1];
    fila[1] = fila[2];
    fila[2] = fila[3];
    fila[3] = fila[4];
    fila[4] = fila[5];
    fila[5] = 0;

    (*cont)--;
    return pidN;
}

int tratar(int pidN, int stats[]){
    int proximo = ((pidN == 6) ? 1 : pidN + 1);

    if(stats[proximo - 1] == 0 || proximo == pid){
        continuar(proximo);
        return proximo;
    }
    else if(stats[proximo - 1] == 1){
        return tratar(proximo, stats);
    }
    else if(stats[proximo - 1] == 2){
        return tratar(proximo, stats);
    }

    return pidN;
}

int main()
{
    int mensagem[2];
    int resp1[2], resp2[2], resp3[2], resp4[2], resp5[2], resp6[2];
    int buf_A1_A2, buf_A2_A1, buf_A3_A4, buf_A4_A3, buf_A5_A6, buf_A6_A5;
    char buf_msg[10];
    Msg msg;

    pipe(mensagem);
    pipe(resp1);
    pipe(resp2);
    pipe(resp3);
    pipe(resp4);
    pipe(resp5);
    pipe(resp6);

    sprintf(buf_msg, "%d", mensagem[1]);

    pid1 = fork();
    if(pid1 == 0){
        close(mensagem[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child 1: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a1", "a1", buf_msg, NULL);
        exit(0);
    }
    close(resp1[0]);

    pid2 = fork();
    if(pid2 == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child 2: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a2", "a2", buf_msg, NULL);
        exit(0);
    }
    close(resp2[0]);
    pausar(2);

    pid3 = fork();
    if(pid3 == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child 3: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a3", "a3", buf_msg, NULL);
        exit(0);
    }
    close(resp3[0]);
    pausar(3);

    pid4 = fork();
    if(pid4 == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child 4: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a4", "a4", buf_msg, NULL);
        exit(0);
    }
    close(resp4[0]);
    pausar(4);

    pid5 = fork();
    if(pid5 == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child 5: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a5", "a5", buf_msg, NULL);
        exit(0);
    }
    close(resp5[0]);
    pausar(5);

    pid6 = fork();
    if(pid6 == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[1]);
        printf("Child 6: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a6", "a6", buf_msg, NULL);
        exit(0);
    }
    close(resp6[0]);
    pausar(6);

    pid_irq = fork();
    if(pid_irq == 0){
        close(mensagem[0]);
        close(resp1[0]);
        close(resp1[1]);
        close(resp2[0]);
        close(resp2[1]);
        close(resp3[0]);
        close(resp3[1]);
        close(resp4[0]);
        close(resp4[1]);
        close(resp5[0]);
        close(resp5[1]);
        close(resp6[0]);
        close(resp6[1]);
        printf("Child IRQ: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./interControllerSim", "interControllerSim", buf_msg, NULL);
        exit(0);
    }
    close(mensagem[1]);

    printf("Parent (KernelSim): PID = %d\n", getpid());

    while(1){
        read(mensagem[0], &msg, sizeof(Msg));

        if(msg.tipo == 0){
            if(msg.irq0 == 1){
                pausar(pid);
                pid = tratar(pid, status);
            }

            if(msg.irq1 == 1 && cont_recv > 0){
                int atual = desenfileirar(fila_recv, &cont_recv);
                status[atual - 1] = 0;
                // Tratar o que deve ser feito quando irq1 é verdadeiro, por enquanto só continua o processo
                continuar(atual);
            }

            if(msg.irq2 == 1 && cont_send > 0){
                int atual = desenfileirar(fila_send, &cont_send);
                status[atual - 1] = 0;
                // Tratar o que deve ser feito quando irq2 é verdadeiro, por enquanto só continua o processo
                continuar(atual);
            }
        }

        else if(msg.tipo == 1){
            // Tratar o que deve ser feito quando um processo envia uma mensagem para outro
        }

        else if(msg.tipo == 2){
            status[msg.origem - 1] = 2;
            finalizados++;
            if(finalizados == 6){
                printf("Todos os processos finalizaram.\n");
                break;
            }
        }
    }

    return 0;
}