#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "constants.h"

pid_t pids[6];
pid_t pid_irq;
int atual = 1;
int cont_recv = 0, fila_recv[6];
int cont_send = 0, fila_send[6];
int status[6];
int finalizados = 0;

int resp[6][2];

// parceiro[6] = {1,0,3,2,5,4} -- mapa fixo A1<->A2, A3<->A4, A5<->A6
// buf_A1_A2 etc viram um array buf[6] indexado por processo
// pc_pendente[6] -- guarda o PC de cada processo ao pedir send(), usado no IRQ2

void pausar(int pidN){
    if(pidN >= 1 && pidN <= 6){
        kill(pids[pidN - 1], SIGSTOP)
    }
}

void continuar(int pidN){
    if(pidN >= 1 && pidN <= 6){
        kill(pids[pidN - 1], SIGCONT)
    }
}

void enfileirar(int pidN, int fila[], int *cont){
    if(*cont < 6){
        fila[*cont] = pidN;
        (*cont)++;
    }
}

int desenfileirar(int fila[], int *cont){
    int pidN = fila[0];

    for(int i = 0; i < *cont - 1; i++){
        fila[i] = fila[i+1];
    }
    fila[*cont - 1] = 0;

    (*cont)--;
    return pidN
}

int tratar(int pidN, int stats[]){
    int proximo = ((pidN == 6) ? 1 : pidN + 1);

    if(stats[proximo - 1] == PRONTO || proximo == pid){
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

int escalonar(void){
    for(int k = 1; k <= 6; k++){
        int cand = ((atual + k -1) % 6) + 1;

        if(status[cand - 1] == PRONTO){
            status[cand - 1] = EXECUTANDO;
            continuar(cand);
            printf("A%d executando\n", cand);

            return cand;
        }
    }

    printf("nenhum processo pronto\n");
    return 0;
}

int main(){
    int mensagem[2];
    char buf_msg[10];
    char *progs[6] = {"./a1", "./a2", "./a3", "./a4", "./a5", "./a6"};
    char *nomes[6] = {"a1", "a2", "a3", "a4", "a5", "a6"};
    Msg msg;

    pipe(mensagem);
    for (int i = 0; i < 6; i++){
        pipe(resp[i]);
    }

    sprintf(buf_msg, "%d", mensagem[1]);

    for(int i = 0; i < 6; i++){
        pids[i] = fork();

        if(pids[i] == 0){
            close(mensagem[0]);
            for(int j = 0; j < 6; j++){
                close(resp[j][1]);
                if(j != i){
                    close(resp[j][0])
                }
            }

            char buf_resp[10];
            sprintf(buf_resp, "%d", resp[i][0]);
            execl(progs[i], nomes[i], buf_msg, buf_resp, NULL);
            perror("execl");

            exit(1);
        }

        close(resp[i][0]);

        if(i == 0){
            status[i] = EXECUTANDO;
        }
        else{
            status[i] = PRONTO;
            pausar(i + 1);
        }
    }

    pid_irq = fork();
    if(pid_irq == 0){
        close(mensagem[0]);
        for(int j = 0; j < 6; j++){
            close(resp[j][1]);
        }
        printf("Child IRQ: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./interControllerSim", "interControllerSim", buf_msg, NULL);
        perror("execl");
        exit(1);
    }
    close(mensagem[1]);

    printf("Parent (KernelSim): PID = %d\n", getpid());

    while(1){
        ssize_t lidos = read(mensagem[0], &msg, sizeof(Msg));

        if(msg.tipo == IRQ){
            if(msg.irq0 == 1){
                if(atual != 0){
                    pausar(atual);
                    if(status[atual - 1] == EXECUTANDO){
                        status[atual - 1] = PRONTO;
                    }
                }
                atual = escalonar();
            }

            if(msg.irq1 == 1 && cont_recv > 0){
                int p = desenfileirar(fila_recv, &cont_recv);
                status[p - 1] = PRONTO;
                printf("A%d desbloqueado (recv)\n", p);

                if(atual == 0){
                    atual = escalonar();
                }
            }

            if(msg.irq2 == 1 && cont_send > 0){
                int p = desenfileirar(fila_send, &cont_send);
                status[p - 1] = PRONTO;
                printf("A%d desbloqueado (send)\n", p);
                
                if(atual == 0){
                    atual = escalonar();
                }
            }
        }

        else if(msg.tipo == SYSCALL){
            // Fase 3: pausar, marcar BLOQUEADO, salvar op/valor,
            // enfileirar e escalonar outro processo
        }

        else if(msg.tipo == FIM){
            status[msg.origem - 1] = TERMINADO;
            finalizados++;
            if(finalizados == 6){
                printf("Todos os processos finalizaram.\n");
                break;
            }

            if(msg.origem == atual){
                atual = escalonar();
            }
        }
    }

    kill(pid_irq, SIGKILL);
    waitpid(pid_irq, NULL, 0);
    for(int i = 0; i < 6; i++){
        waitpid(pids[i], NULL, 0);
    }

    return 0;
}