#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX 20

/* Operações de syscall */
#define R 0   // recv (leitura)
#define W 1   // send (escrita)

/* Tipos de mensagem (campo Msg.tipo) */
#define IRQ     0   // InterController -> Kernel
#define SYSCALL 1   // Aplicação -> Kernel
#define FIM     2   // Aplicação avisa que terminou

/* Estados dos processos */
#define PRONTO     0
#define EXECUTANDO 1
#define BLOQUEADO  2
#define TERMINADO  3

typedef struct {
    int tipo;     // IRQ, SYSCALL ou FIM
    int irq0;     // usados quando tipo == IRQ
    int irq1;
    int irq2;
    int origem;   // id da aplicação (1..6), quando tipo == SYSCALL ou FIM
    int op;       // R ou W, quando tipo == SYSCALL
    int valor;    // PC do processo, quando tipo == SYSCALL
} Msg;

#endif