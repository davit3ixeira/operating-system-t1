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

// TODO: parceiro[6] = {1,0,3,2,5,4} -- mapa fixo A1<->A2, A3<->A4, A5<->A6
// TODO: buf_A1_A2 etc viram um array buf[6] indexado por processo
// TODO: pc_pendente[6] -- guarda o PC de cada processo ao pedir send(), usado no IRQ2

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

// TODO: funcao pra receber syscall de um Ai -- le fdN[0], identifica escrever ou ler,
// da SIGSTOP, marca status BLOCKED, enfileira, e guarda PC se for send

int main()
{
    int fd1[2], fd2[2], fd3[2], fd4[2], fd5[2], fd6[2], irq_fd[2];
    int buf_A1_A2, buf_A2_A1, buf_A3_A4, buf_A4_A3, buf_A5_A6, buf_A6_A5; // TODO: virar array, ver topo do arquivo
    char buf_irq[10];
    IrqMsg msg;

    // TODO: falta criar pipes de RESPOSTA (Kernel -> Ai), um por processo

    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    pipe(fd4);
    pipe(fd5);
    pipe(fd6);
    pipe(irq_fd);

    pid1 = fork();
    if(pid1 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 1: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a1", "a1", NULL);
        exit(0);
    }
    close(fd1[1]);

    pid2 = fork();
    if(pid2 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd3[1]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 2: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a2", "a2", NULL);
        exit(0);
    }
    close(fd2[1]);

    pid3 = fork();
    if(pid3 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd4[1]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 3: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a3", "a3", NULL);
        exit(0);
    }
    close(fd3[1]);

    pid4 = fork();
    if(pid4 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd5[1]);
        close(fd6[0]);
        close(fd6[1]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 4: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a4", "a4", NULL);
        exit(0);
    }
    close(fd4[1]);

    pid5 = fork();
    if(pid5 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd6[0]);
        close(fd6[1]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 5: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a5", "a5", NULL);
        exit(0);
    }
    close(fd5[1]);

    pid6 = fork();
    if(pid6 == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd6[0]);
        close(irq_fd[0]);
        close(irq_fd[1]);
        printf("Child 6: PID = %d, PPID = %d\n", getpid(), getppid());
        execl("./a6", "a6", NULL);
        exit(0);
    }
    close(fd6[1]);

    pid_irq = fork();
    if(pid_irq == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd3[0]);
        close(fd4[0]);
        close(fd5[0]);
        close(fd6[0]);
        close(irq_fd[0]);
        printf("Child IRQ: PID = %d, PPID = %d\n", getpid(), getppid());

        sprintf(buf_irq, "%d", irq_fd[1]);
        execl("./interControllerSim", "interControllerSim", buf_irq, NULL);
        exit(0);
    }
    close(irq_fd[1]);

    printf("Parent (KernelSim): PID = %d\n", getpid());

    // TODO: A2-A6 nascem todos rodando -- pausar todos exceto A1 antes do while

    while(1){
        // TODO: so escuta irq_fd -- precisa saber tambem de fd1[0]..fd6[0]
        read(irq_fd[0], &msg, sizeof(IrqMsg));

        if(msg.irq0 == 1){
            pausar(pid);
            pid = tratar(pid, status);
        }

        if(msg.irq1 == 1 && cont_recv > 0){
            int atual = desenfileirar(fila_recv, &cont_recv);
            status[atual - 1] = 0;
            // TODO: ler buffer do parceiro de 'atual' e entregar como N (falta canal de resposta)
            continuar(atual);
        }

        if(msg.irq2 == 1 && cont_send > 0){
            int atual = desenfileirar(fila_send, &cont_send);
            status[atual - 1] = 0;
            // TODO: escrever pc_pendente[atual] no buffer dele
            continuar(atual);
        }
    }

    // TODO: while(1) nunca sai -- decidir como detectar todos os Ai terminados (waitpid) e encerrar

    return 0;
}